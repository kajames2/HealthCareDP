#ifndef _MODIFY_STRATEGY_HARVEST_H_
#define _MODIFY_STRATEGY_HARVEST_H_

#include "modify_strategy.h"
#include "harvest.h"
#include "dp_state.h"

#include <memory>

namespace healthcaredp {
template <class T>
class ModifyStrategyHarvest : public genericdp::ModifyStrategy<T> {
 public:
  explicit ModifyStrategyHarvest(std::shared_ptr<const healthcare::Harvest> harvest);
  void Modify(genericdp::DPState<T>* state) const override;
 private:
  std::shared_ptr<const healthcare::Harvest> harvest_;
};

template <class T>
ModifyStrategyHarvest<T>::ModifyStrategyHarvest(
    std::shared_ptr<const healthcare::Harvest> harvest)
    : harvest_(harvest) {}

template <class T>
void ModifyStrategyHarvest<T>::Modify(genericdp::DPState<T> *state) const {
  state->domain.harvest = harvest_->GetHarvest(state->domain);
  state->domain.total_working_harvest += harvest_->GetWorkingHarvest(state->domain);
  state->domain.cash += state->domain.harvest;
}

}  // namespace healthcaredp

#endif //_MODIFY_STRATEGY_HARVEST_H_
