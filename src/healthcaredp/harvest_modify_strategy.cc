#include "harvest_modify_strategy.h"

namespace healthcaredp {

HarvestModifyStrategy::HarvestModifyStrategy(
    std::shared_ptr<const healthcare::Harvest> harvest)
    : harvest_(harvest) {}

void HarvestModifyStrategy::Modify(DPHealthState *state) const {
  state->harvest = harvest_->GetHarvest(*state);
  state->total_working_harvest += harvest_->GetWorkingHarvest(*state);
  state->cash += state->harvest;
}
}
