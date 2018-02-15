#ifndef _MODIFY_STRATEGY_ACCOUNTING_H_
#define _MODIFY_STRATEGY_ACCOUNTING_H_

#include "dp_state.h"
#include "modify_strategy.h"

#include <memory>

namespace healthcaredp {
template <class T>
class ModifyStrategyAccounting : public genericdp::ModifyStrategy<T> {
public:
  ModifyStrategyAccounting();
  void Modify(genericdp::DPState<T> *state) const override;
};

template <class T> ModifyStrategyAccounting<T>::ModifyStrategyAccounting() {}

template <class T>
void ModifyStrategyAccounting<T>::Modify(genericdp::DPState<T> *state) const {
  state->domain.end_health = state->domain.health;
  state->domain.end_cash = state->domain.cash;
  state->immediate_value = state->domain.life_enjoyment;
  state->domain.period++;
}

} // namespace healthcaredp

#endif //_MODIFY_STRATEGY_ACCOUNTING_H_
