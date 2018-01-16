#include "consumption_modify_strategy.h"

namespace healthcaredp {

ConsumptionModifyStrategy::ConsumptionModifyStrategy(
    std::shared_ptr<const healthcare::Consumption> consumption)
    : consumption_(consumption) {}

void ConsumptionModifyStrategy::Modify(DPHealthState *state) const {
  if (IsAlive(*state)) {
    state->life_enjoyment = consumption_->GetLifeEnjoyment(state->health, state->life_investment);    
  } else {
    state->life_enjoyment = 0;
  }
}
}
