#include "degeneration_modify_strategy.h"

namespace healthcaredp {

DegenerationModifyStrategy::DegenerationModifyStrategy(
    std::shared_ptr<const healthcare::Degeneration> degen)
    : degen_(degen) {}

void DegenerationModifyStrategy::Modify(DPHealthState *state) const {
  state->health -= degen_->GetDegeneration(state->period);
}
}
