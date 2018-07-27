#include "genericdp/result.h"
#include "genericdp/result_set.h"
#include "healthcare/consumption/fractional.h"
#include "healthcare/consumption/increasing_decay.h"
#include "healthcare/consumption/precompute_decorator.h"
#include "healthcare/decision.h"
#include "healthcare/degeneration/linear.h"
#include "healthcare/degeneration/precompute_decorator.h"
#include "healthcare/job/flat.h"
#include "healthcare/job/linear.h"
#include "healthcare/person.h"
#include "healthcare/probability/age_defined.h"
#include "healthcare/regeneration/logistic.h"
#include "healthcare/regeneration/precompute_decorator.h"
#include "healthcare/regeneration/shifted_logistic.h"
#include "healthcare/regeneration/shifted_logistic_rounded.h"
#include "healthcaredp/fastdp/decision_cache.h"
#include "healthcaredp/storage.h"

#include <omp.h>
#include <iostream>
#include <memory>
#include <utility>

using namespace healthcare;
using namespace healthcaredp;
using namespace genericdp;
using DPPerson = healthcare::Person;
using DPPolicy = healthcare::Decision;
using PersonPolicy = genericdp::Result<DPPerson, DPPolicy>;
using DPStorage = healthcaredp::Storage<DPPerson, DPPolicy>;

constexpr int max_age = 9;
constexpr int max_savings = 150;
constexpr int min_savings = -80;
constexpr int max_budget = max_savings + 100;
constexpr float discount = 1;
const job::Linear work(0.93622);
const regeneration::PrecomputeDecorator regen(regeneration::Logistic(30, 0.025,
                                                                     0),
                                              max_budget);
const degeneration::Linear degen(15, 1);
const consumption::PrecomputeDecorator consume(
    consumption::IncreasingDecay(0.028, 0.5, 500));

void PrintOptimal(const DPStorage& storage, DPPerson state);
PersonPolicy GetOptimalPolicy(const std::vector<PersonPolicy>& dec_states,
                              const DPStorage& storage);
std::vector<DPPerson> CreateTemplateStates();
void PrePolicy(std::vector<DPPerson>& states);
PersonPolicy ApplyPolicy(DPPerson state, DPPolicy dec);

void StoreOptimalPolicys(DPStorage& storage);
void StoreAgeOptimals(DPStorage& storage,
                      const fastdp::DecisionCache& dec_cache,
                      const std::vector<DPPerson>& init_states);
void StoreAgeOptimalsShocks(DPStorage& storage,
                            const fastdp::DecisionCache& dec_cache,
                            const std::vector<DPPerson>& init_states);

int main() {
  DPStorage opt_policys(max_age + 1, max_savings, min_savings);
  StoreOptimalPolicys(opt_policys);
  std::cout << opt_policys << std::endl;

  //DPPerson state{1, 85, 0};
  //PrintOptimal(opt_policys, state);
}

void PrintOptimal(const DPStorage& storage, DPPerson state) {
  std::cout << "Age,Health,Cash,NextAge,NextHealth,NextCash,HealthSpending,"
               "JoySpending,Probability,"
               "Enjoyment,FutureValue,Value\n";
  for (unsigned int i = 1; i <= max_age; ++i) {
    auto res = storage.GetOptimalResult(state);
    std::cout << state << "," << res << '\n';
    state = res.state;
  }
  std::cout << std::endl;
}

std::vector<DPPerson> CreateTemplateStates() {
  std::vector<DPPerson> init_states;
  init_states.reserve((100 + 1) * (max_savings + 1));
  for (int h = 0; h <= 100; ++h) {
    for (int c = min_savings; c <= max_savings; ++c)
      init_states.push_back(DPPerson{0, h, c});
  }
  return init_states;
}

void StoreOptimalPolicys(DPStorage& storage) {
  fastdp::DecisionCache dec_cache(max_savings, max_budget, regen);
  std::vector<DPPerson> init_states = CreateTemplateStates();
  std::vector<DPPerson> states(init_states.size());
  for (int age = max_age; age >= 1; --age) {
    for (auto& state : init_states) {
      state.age = age;
    }
    StoreAgeOptimals(storage, dec_cache, init_states);
  }
}

void StoreAgeOptimals(DPStorage& storage,
                      const fastdp::DecisionCache& dec_cache,
                      const std::vector<DPPerson>& init_states) {
  auto states = init_states;
  PrePolicy(states);
  std::vector<PersonPolicy> dec_states(max_budget * 100);
// Performance critical loop.  Be careful when modifying.
#pragma omp parallel for private(dec_states)
  for (int cur = 0; cur < states.size(); ++cur) {
    DPPerson cur_state = states[cur];
    auto start_it = dec_cache.begin(cur_state.health, cur_state.cash);
    auto end_it = dec_cache.end(cur_state.health, cur_state.cash);
    size_t n_policys = std::distance(start_it, end_it);
    dec_states.resize(n_policys);
    std::transform(start_it, end_it, dec_states.begin(),
                   [cur_state](DPPolicy dec) -> PersonPolicy {
                     return ApplyPolicy(cur_state, dec);
                   });

    PersonPolicy opt_dec;
    for (PersonPolicy state : dec_states) {
      state.valuation.value += storage.GetOptimalValue(state.state);
      if (state.valuation.value > opt_dec.valuation.value) {
        opt_dec = state;
      }
    }
    storage.StoreOptimalResult(init_states[cur], opt_dec,
                               opt_dec.valuation.value);
  }
}

void PrePolicy(std::vector<DPPerson>& states) {
  for (auto& state : states) {
    state.cash += work.GetEarnings(state.age, state.health);
    state.health -= degen.GetDegeneration(state.age, state.health);
    state.cash = std::max(state.cash, 0);
    state.health = std::max(state.health, 0);
  }
}

PersonPolicy ApplyPolicy(DPPerson state, DPPolicy policy) {
  genericdp::Valuation val;

  state.cash -= TotalSpending(policy);
  state.cash = std::min(max_savings, state.cash);
  state.health += regen.GetHealthRegained(state.health, policy.health_spending);
  val.immediate_value =
      consume.GetLifeEnjoyment(state.health, policy.joy_spending);
  val.value = val.immediate_value;
  ++state.age;
  return PersonPolicy{val, state, policy};
}
