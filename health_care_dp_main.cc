#include "accounting_modify_strategy.h"
#include "consumption_modify_strategy.h"
#include "decision_dp_stage.h"
#include "degeneration_modify_strategy.h"
#include "dp_result.h"
#include "dp_template.h"
#include "exogenous_dp_stage.h"
#include "fixed_probablity_strategy.h"
#include "flat_harvest.h"
#include "fractional_consumption.h"
#include "harvest_modify_strategy.h"
#include "health_decision_iterator_factory.h"
#include "health_dp_storage.h"
#include "increasing_decay_consumption.h"
#include "linear_degeneration.h"
#include "linear_harvest.h"
#include "log_logistic_consumption.h"
#include "logistic_consumption.h"
#include "logistic_regeneration.h"
#include "modify_strategy_composite.h"
#include "modify_strategy_set.h"
#include "modify_vector_iterator_factory.h"
#include "regeneration_modify_strategy.h"
#include "shifted_log_logistic_regeneration.h"
#include "shifted_logistic_regeneration.h"
#include "shock_modify_strategy.h"
#include "stochastic_dp_stage.h"
#include "value_strategy.h"

#include <exception>
#include <iostream>
#include <memory>
#include <utility>

using namespace healthcare;
using namespace healthcaredp;
using namespace genericdp;

void PrintState(DPHealthState state) {
  std::cout << state.period - 1 << ", " << state.harvest << ", "
            << state.investment_cash << ", " << state.investment_health << ", "
            << state.health_investment << ", " << state.life_investment << ", "
            << state.end_cash << ", " << state.end_health << ", "
            << state.life_enjoyment << ", " << state.shock << ", "
            << state.probability << ", " << state.immediate_value << ", "
            << state.future_value << ", " << state.value << std::endl;
}

int main() {
  int n_periods = 20;
  int max_remaining_cash = 100;

  auto harvest_ = std::make_shared<LinearHarvest>(1, n_periods, 0.8);

  auto degen_ = std::make_shared<LinearDegeneration>(10);
  auto regen_ = std::make_shared<const ShiftedLogisticRegeneration>(0.01021);
  auto consume_ = std::make_shared<const FractionalConsumption>(32);
  // auto regen_ =
  //     std::make_shared<const ShiftedLogLogisticRegeneration>(
  //         60, 1, max_investment_cash);

  auto shock_ =
      std::make_shared<genericdp::ModifyStrategyComposite<DPHealthState>>();
  shock_->AddStrategy(std::make_shared<const ShockModifyStrategy>(true));
  shock_->AddStrategy(std::make_shared<const DegenerationModifyStrategy>(
      std::make_shared<LinearDegeneration>(30)));
  auto no_shock_ = std::make_shared<const ShockModifyStrategy>(false);
  auto shock_prob_ =
      std::make_shared<const FixedProbabilityStrategy<DPHealthState>>(0.2);
  ModifyStrategySet<DPHealthState> shock_set_(shock_, no_shock_, shock_prob_);

  auto value_strat = std::make_shared<ValueStrategy<DPHealthState>>(1);

  auto degen = std::make_unique<DegenerationModifyStrategy>(degen_);
  auto degen_stage =
      std::make_unique<ExogenousDPStage<DPHealthState>>(std::move(degen));
  auto harvest = std::make_unique<HarvestModifyStrategy>(harvest_);
  auto harvest_stage =
      std::make_unique<ExogenousDPStage<DPHealthState>>(std::move(harvest));
  auto decision_it_fact =
      std::make_unique<HealthDecisionIteratorFactory>(max_remaining_cash);
  auto decision_stage = std::make_unique<DecisionDPStage<DPHealthState>>(
      std::move(decision_it_fact));
  auto regen = std::make_unique<RegenerationModifyStrategy>(regen_);
  auto regen_stage =
      std::make_unique<ExogenousDPStage<DPHealthState>>(std::move(regen));
  auto consume = std::make_unique<ConsumptionModifyStrategy>(consume_);
  auto consume_stage =
      std::make_unique<ExogenousDPStage<DPHealthState>>(std::move(consume));
  auto shock_fact =
      std::make_unique<ModifyVectorIteratorFactory<DPHealthState>>(shock_set_);
  auto shock_stage =
      std::make_unique<StochasticDPStage<DPHealthState>>(std::move(shock_fact));
  auto stages = std::vector<std::unique_ptr<DPStage<DPHealthState>>>();
  auto accounting_stage = std::make_unique<ExogenousDPStage<DPHealthState>>(
      std::make_unique<AccountingModifyStrategy>());

  stages.push_back(std::move(harvest_stage));
  stages.push_back(std::move(degen_stage));
  stages.push_back(std::move(shock_stage));
  stages.push_back(std::move(decision_stage));
  stages.push_back(std::move(regen_stage));
  stages.push_back(std::move(consume_stage));
  stages.push_back(std::move(accounting_stage));

  std::unique_ptr<DPStorage<DPHealthState>> storage;
  storage = std::make_unique<HealthDPStorage>(n_periods, max_remaining_cash);

  DPTemplate<DPHealthState> health_dp(std::move(storage), std::move(stages),
                                      value_strat);

  // HealthStateIterator state_it(n_periods, max_remaining_cash);
  // health_dp.BottomUpTrain(state_it);
  DPHealthState state;
  state.period = 1;
  state.cash = 0;
  state.health = 85;
  state.total_working_harvest = 0;
  std::cout << "period" << ", "
            << "harvest" << ", "
            << "investment_cash" << ", "
            << "investment_health" << ", "
            << "health_investment" << ", "
            << "life_investment" << ", "
            << "end_cash" << ", "
            << "end_health" << ", "
            << "life_enjoyment" << ", "
            << "shocked?" << ", "
            << "probability" << ", "
            << "immediate_value" << ", "
            << "future_value" << ", "
            << "value" << std::endl;

  for (int i = 0; i < n_periods; ++i) {
    auto solution = health_dp.GetOptimalResult(state);
    state = solution[1];
    PrintState(state);
  }
}
