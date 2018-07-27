#include "healthcaredp/decision_cache_fitness.h"
#include "healthcare/decision_fitness.h"
#include "healthcare/fitness.h"
#include "healthcare/insurance.h"
#include "healthcare/regeneration.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

namespace healthcaredp {
namespace fastdp {

using namespace healthcare;

DecisionCacheFitness::DecisionCacheFitness(int age, Configuration config)
    : config_(config),
      counts_(BuildCounts(age)),
      decisions_(BuildDecisions(age)) {}

std::vector<std::vector<int>> DecisionCacheFitness::BuildCounts(int age) const {
  std::vector<std::vector<int>> counts;
  counts.reserve(101);
  for (int health = 0; health <= 100; ++health) {
    counts.push_back(BuildHealthCounts(age, health));
  }
  return counts;
}

std::vector<int> DecisionCacheFitness::BuildHealthCounts(int age,
                                                         int health) const {
  int n_choices = 0;
  std::vector<int> counts(config_.max_budget + 1);
  if (health == 0) {
    std::fill(counts.begin(), counts.end(), 1);
    return counts;
  }
  for (int budget = 0; budget <= config_.max_budget; ++budget) {
    counts[budget] = NOptionsExactBudget(age, health, budget);
  }
  std::partial_sum(counts.begin(), counts.end(), counts.begin());
  return counts;
}

int DecisionCacheFitness::NOptionsExactBudget(int age, int health,
                                              int budget) const {
  int max_health =
      std::min(100 - health, config_.regen->GetHealthRegained(health, budget));
  int count = 0;
  for (int hr = 0; hr <= max_health; ++hr) {
    count += NOptionsExactBudgetRegain(
        age, health + hr, budget - config_.regen->GetRegainCost(health, hr));
  }
  return count;
}

int DecisionCacheFitness::NOptionsExactBudgetRegain(int age, int health,
                                                    int budget) const {
  int max_fitness = config_.fitness->GetFitness(budget);
  int count = 0;
  for (int fit = 0; fit <= max_fitness; ++fit) {
    count += NOptionsExactBudgetRegainFit(
        age, health, budget - config_.fitness->GetCost(fit), fit);
  }
  return count;
}

int DecisionCacheFitness::NOptionsExactBudgetRegainFit(int age, int health,
                                                       int budget,
                                                       int fitness) const {
  return 1 + (config_.insure->GetCost(age, fitness, health) <= budget ? 1 : 0);
}

std::vector<std::vector<DecisionFitness>> DecisionCacheFitness::BuildDecisions(
    int age) const {
  std::vector<std::vector<DecisionFitness>> decs;
  for (int health = 0; health <= 100; ++health) {
    decs.push_back(BuildHealthDecisions(age, health));
  }
  return decs;
}

std::vector<DecisionFitness> DecisionCacheFitness::BuildHealthDecisions(
    int age, int health) const {
  if (health == 0) {
    return std::vector<DecisionFitness>(1);
  }
  std::vector<DecisionFitness> decs;
  int max_health = std::min(100 - health, config_.regen->GetHealthRegained(
                                              health, config_.max_budget));
  for (int hr = 0; hr <= max_health; ++hr) {
    int regen_cost = config_.regen->GetRegainCost(health, hr);
    int max_fitness =
        config_.fitness->GetFitness(config_.max_budget - regen_cost);
    for (int fit = 0; fit <= max_fitness; ++fit) {
      int fit_cost = config_.fitness->GetCost(fit);
      int rem_budget = config_.max_budget - regen_cost - fit_cost;
      for (int joy_spending = 0; joy_spending <= rem_budget; ++joy_spending) {
        decs.push_back(DecisionFitness{regen_cost, joy_spending, fit_cost, 0});
      }
      int insure_cost = config_.insure->GetCost(age, fit, health + hr);
      if (insure_cost <= rem_budget) {
        for (int joy_spending = 0; joy_spending <= rem_budget - insure_cost;
             ++joy_spending) {
          decs.push_back(
              DecisionFitness{regen_cost, joy_spending, fit_cost, insure_cost});
        }
      }
    }
  }
  std::stable_sort(decs.begin(), decs.end(),
                   [](DecisionFitness dec, DecisionFitness dec2) {
                     return TotalSpending(dec) < TotalSpending(dec2);
                   });
  return decs;
}

}  // namespace fastdp
}  // namespace healthcaredp
