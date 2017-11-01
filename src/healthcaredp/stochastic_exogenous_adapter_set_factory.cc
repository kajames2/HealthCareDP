#include "stochastic_exogenous_adapter_set_factory.h"

namespace healthcaredp {
using StochasticInvestmentState =
    StochasticExogenousAdapterSetFactory::StochasticInvestmentState;

StochasticExogenousAdapterSetFactory::StochasticExogenousAdapterSetFactory(
    std::vector<healthcaredp::StochasticExogenousAdapterFactory> fact_set)
    : fact_set_(std::move(fact_set)) {}

std::vector<StochasticInvestmentState>
StochasticExogenousAdapterSetFactory::GetExogenousSet(
    const healthcare::HealthState &state) const {
  std::vector<StochasticInvestmentState> state_set;
  for (auto &fact : fact_set_) {
    state_set.push_back(std::move(fact.GetStochasticExogenous(state)));
  }
  return std::move(state_set);
}
}
