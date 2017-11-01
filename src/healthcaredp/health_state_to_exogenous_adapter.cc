#include "health_state_to_exogenous_adapter.h"
#include "exogenous_state.h"
#include "health_state.h"

namespace healthcaredp{
std::unique_ptr<genericdp::ExogenousState<healthcare::HealthState>>
HealthStateToExogenousAdapter::Clone() const{
    return std::make_unique<HealthStateToExogenousAdapter>(state_);
}


std::string
HealthStateToExogenousAdapter::GetHeader() const {
  return std::string("Period") + ", "
      + "Inv. Health" + ", "
      + "Inv. Cash";
}

std::string
HealthStateToExogenousAdapter::GetString() const {
  return std::to_string(state_.period) + ", " +
         std::to_string(state_.health) + ", " +
      std::to_string(state_.cash);
}
} // namespace healthcaredp
