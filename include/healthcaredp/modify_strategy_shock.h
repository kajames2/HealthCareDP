#ifndef _MODIFY_STRATEGY_SHOCK_H_
#define _MODIFY_STRATEGY_SHOCK_H_

#include "dp_state.h"
#include "modify_strategy.h"
#include "consumption.h"
#include <memory>

namespace healthcaredp {
template <class T>
class ModifyStrategyShock : public genericdp::ModifyStrategy<T> {
 public:
  ModifyStrategyShock(bool is_shocked) : is_shocked_(is_shocked) {}
  void Modify(genericdp::DPState<T>* state) const override {state->domain.shock = is_shocked_;}
 private:
  bool is_shocked_;
};

}  // namespace healthcaredp

#endif //_MODIFY_STRATEGY_SHOCK_H_
