#ifndef _MODIFY_STRATEGY_CONSUMPTION_H_
#define _MODIFY_STRATEGY_CONSUMPTION_H_

#include "health_state.h"
#include "consumption.h"
#include "dp_state.h"
#include "modify_strategy.h"

#include <memory>

namespace healthcaredp {
template <class T>
class ModifyStrategyConsumption : public genericdp::ModifyStrategy<T> {
public:
  explicit ModifyStrategyConsumption(
      std::shared_ptr<const healthcare::Consumption> consumption);
  void Modify(genericdp::DPState<T> *state) const override;

private:
  std::shared_ptr<const healthcare::Consumption> consumption_;
};

template <class T>
ModifyStrategyConsumption<T>::ModifyStrategyConsumption(
    std::shared_ptr<const healthcare::Consumption> consumption)
    : consumption_(consumption) {}

template <class T>
void ModifyStrategyConsumption<T>::Modify(genericdp::DPState<T> *state) const {
  if (IsAlive(state->domain)) {
    state->domain.life_enjoyment = consumption_->GetLifeEnjoyment(
        state->domain.health, state->domain.life_investment);
  } else {
    state->domain.life_enjoyment = 0;
  }
}

} // namespace healthcaredp

#endif //_MODIFY_STRATEGY_CONSUMPTION_H_
