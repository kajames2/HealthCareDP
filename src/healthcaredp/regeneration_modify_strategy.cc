#include "regeneration_modify_strategy.h"

#include <algorithm>

namespace healthcaredp {

RegenerationModifyStrategy::RegenerationModifyStrategy(
    std::shared_ptr<const healthcare::Regeneration> regen)
    : regen_(regen) {}

void RegenerationModifyStrategy::Modify(DPHealthState *state) const {
  if (IsAlive(*state)) {
    state->health += regen_->GetHealthRegained(state->health, state->health_investment);
    state->health = std::min(state->health, 100);
  }
}
}
