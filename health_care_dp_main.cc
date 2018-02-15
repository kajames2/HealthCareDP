#include "consumption_fractional.h"
#include "consumption_increasing_decay.h"
#include "consumption_log_logistic.h"
#include "consumption_logistic.h"
#include "decision_state.h"
#include "degeneration_linear.h"
#include "dp.h"
#include "dp_result.h"
#include "harvest_flat.h"
#include "harvest_linear.h"
#include "health_decision_iterator_factory.h"
#include "health_dp_storage.h"
#include "modify_strategy_accounting.h"
#include "modify_strategy_composite.h"
#include "modify_strategy_consumption.h"
#include "modify_strategy_degeneration.h"
#include "modify_strategy_harvest.h"
#include "modify_strategy_regeneration.h"
#include "modify_strategy_set.h"
#include "modify_strategy_shock.h"
#include "modify_vector_iterator_factory.h"
#include "probability_strategy_fixed.h"
#include "regeneration_logistic.h"
#include "regeneration_shifted_log_logistic.h"
#include "regeneration_shifted_logistic.h"
#include "stage_decision.h"
#include "stage_exogenous.h"
#include "value_strategy.h"

#include <exception>
#include <iostream>
#include <memory>
#include <utility>

using namespace healthcare;
using namespace healthcaredp;
using namespace genericdp;

// std::string GetStateString(FitnessDPHealthState state) {
//   std::vector<std::string> fitnesses{"LOW", "MEDIUM", "HIGH"};
//   return ", " + fitnesses[state.fitness] + ", " +
//          std::to_string(state.insured) + ", " + std::to_string(state.shock);
// }

// std::string GetStateString(ShockDPHealthState state) {
//   return ", " + std::to_string(state.insured) + ", " +
//          std::to_string(state.shock);
// }

std::string GetStateString(DecisionState state) { return ""; }

std::string GetHeaderStart() {
  return std::string("period") + ", " +
      "harvest" + ", " +
      "investment_cash" + ", " +
      "investment_health" + ", " +
      "health_investment" + ", " +
      "life_investment" + ", " +
      "end_cash" + ", " +
      "end_health" + ", " +
      "life_enjoyment";
}

// std::string GetTypeHeader(FitnessDPHealthState state) {
//   return std::string(", ") + "fitness" + ", " + "insured?" + ", " +
//   "shocked?";
// }

// std::string GetTypeHeader(ShockDPHealthState state) {
//   return std::string(", ") + "insured?" + ", " + "shocked?";
// }

std::string GetTypeHeader(DecisionState state) { return ""; }
std::string GetHeaderEnd() {
  return std::string(", ") +
      "probability" + ", " +
      "immediate_value" + ", " +
      "future_value" + ", " +
      "value";
}

int main() {

  using State = DecisionState;

  int n_periods = 10;
  int max_savings = 50;

  auto harvest_ = std::make_shared<HarvestLinear>(1, n_periods, 0.8);

  auto degen_ = std::make_shared<DegenerationLinear>(10);
  auto regen_ = std::make_shared<const RegenerationShiftedLogistic>(0.01021);
  auto consume_ = std::make_shared<const ConsumptionFractional>(32);
  // auto regen_ =
  //     std::make_shared<const ShiftedLogLogisticRegeneration>(
  //         60, 1, max_investment_cash);

  // auto shock_ =
  // std::make_shared<genericdp::ModifyStrategyComposite<State>>();
  // shock_->AddStrategy(std::make_shared<const
  // ModifyStrategyShock<State>>(true));
  // shock_->AddStrategy(std::make_shared<const
  // DegenerationModifyStrategy<State>>(
  //     std::make_shared<DegenerationLinear>(30)));
  // auto no_shock_ = std::make_shared<const ShockModifyStrategy<State>>(false);
  // auto shock_prob_ =
  //     std::make_shared<const FixedProbabilityStrategy<State>>(0.2);
  // ModifyStrategySet<State> shock_set_(shock_, no_shock_, shock_prob_);

  auto value_strat = std::make_shared<ValueStrategy<State>>(1);

  auto degen = std::make_unique<ModifyStrategyDegeneration<State>>(degen_);
  auto degen_stage = std::make_unique<StageExogenous<State>>(std::move(degen));
  auto harvest = std::make_unique<ModifyStrategyHarvest<State>>(harvest_);
  auto harvest_stage =
      std::make_unique<StageExogenous<State>>(std::move(harvest));
  auto decision_it_fact =
      std::make_unique<HealthDecisionIteratorFactory<State>>(max_savings);
  auto decision_stage =
      std::make_unique<StageDecision<State>>(std::move(decision_it_fact));
  auto regen = std::make_unique<ModifyStrategyRegeneration<State>>(regen_);
  auto regen_stage = std::make_unique<StageExogenous<State>>(std::move(regen));
  auto consume = std::make_unique<ModifyStrategyConsumption<State>>(consume_);
  auto consume_stage =
      std::make_unique<StageExogenous<State>>(std::move(consume));
  // auto shock_fact =
  //     std::make_unique<ModifyVectorIteratorFactory<State>>(shock_set_);
  // auto shock_stage =
  //     std::make_unique<StageStochastic<State>>(std::move(shock_fact));
  auto accounting_stage = std::make_unique<StageExogenous<State>>(
      std::make_unique<ModifyStrategyAccounting<State>>());

  auto stages = std::vector<std::unique_ptr<Stage<State>>>();
  stages.push_back(std::move(harvest_stage));
  stages.push_back(std::move(degen_stage));
  // stages.push_back(std::move(shock_stage));
  stages.push_back(std::move(decision_stage));
  stages.push_back(std::move(regen_stage));
  stages.push_back(std::move(consume_stage));
  stages.push_back(std::move(accounting_stage));

  std::unique_ptr<DPStorage<State>> storage;
  storage = std::make_unique<HealthDPStorage<State>>(n_periods, max_savings);

  DP<State> health_dp(std::move(storage), std::move(stages), value_strat);

  State state;
  state.period = 1;
  state.cash = 0;
  state.health = 85;
  state.total_working_harvest = 0;
  std::cout << GetHeaderStart() << GetTypeHeader(state) << GetHeaderEnd()
            << std::endl;

  auto shocks = std::vector<int>(n_periods, 0);
  // shocks[4] = 0;
  // shocks[8] = 0;
  for (int i = 0; i < n_periods; ++i) {
    auto solution = health_dp.GetOptimalResult(state);
    auto dp_state = solution[shocks[i]];
    state = dp_state.domain;
    std::cout << state.period - 1 << ", " <<
        state.harvest << ", " <<
        state.investment_cash << ", " <<
        state.investment_health << ", " <<
        state.health_investment << ", " <<
        state.life_investment << ", " <<
        state.end_cash << ", " <<
        state.end_health << ", " <<
        state.life_enjoyment;
    std::cout << GetStateString(state);
    std::cout << ", " <<
        dp_state.probability << ", " <<
        dp_state.immediate_value << ", " <<
        dp_state.future_value << ", " <<
        dp_state.value << std::endl;
  }
}
