#include "stochastic_exogenous_adapter_factory.h"
#include "health_state_to_exogenous_adapter_factory.h"

namespace healthcaredp {
using StochasticInvestmentState =
    StochasticExogenousAdapterFactory::StochasticInvestmentState;
StochasticExogenousAdapterFactory::StochasticExogenousAdapterFactory(
    std::unique_ptr<const genericdp::ExogenousFactory<healthcare::HealthState>> fact,
    double prob)
    : fact_(std::move(fact)), probability_(prob) {}

StochasticInvestmentState StochasticExogenousAdapterFactory::GetStochasticExogenous(
    const healthcare::HealthState &state) const {
  return StochasticInvestmentState(fact_->GetExogenous(state), probability_);
}
}
