#include "health_state_to_exogenous_adapter_factory.h"
#include "degeneration.h"
#include "harvest.h"
#include "health_investment_state_factory.h"
#include "health_state.h"
#include "health_state_to_exogenous_adapter.h"

#include <memory>

namespace healthcaredp {
HealthStateToExogenousAdapterFactory::HealthStateToExogenousAdapterFactory(
    healthcare::HealthInvestmentStateFactory investment_fact)
    : investment_fact_(investment_fact) {}

HealthStateToExogenousAdapterFactory::HealthStateToExogenousAdapterFactory(
    std::shared_ptr<const healthcare::Harvest> harvest,
    std::shared_ptr<const healthcare::Degeneration> degen)
    : investment_fact_(
          healthcare::HealthInvestmentStateFactory(harvest, degen)) {}

std::unique_ptr<genericdp::ExogenousState<healthcare::HealthState>>
HealthStateToExogenousAdapterFactory::GetExogenous(
    const healthcare::HealthState &state) const {
  return std::make_unique<HealthStateToExogenousAdapter>(
      investment_fact_.GetInvestmentState(state));
};
}
