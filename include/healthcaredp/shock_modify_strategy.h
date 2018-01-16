#ifndef _SHOCK_MODIFY_STRATEGY_H_
#define _SHOCK_MODIFY_STRATEGY_H_

#include "modify_strategy.h"
#include "dp_health_state.h"
#include "consumption.h"
#include <memory>

namespace healthcaredp {
class ShockModifyStrategy : public genericdp::ModifyStrategy<DPHealthState> {
 public:
  ShockModifyStrategy(bool is_shocked) : is_shocked_(is_shocked) {}
  void Modify(DPHealthState* state) const override {state->shock = is_shocked_;}
 private:
  bool is_shocked_;
};

}  // namespace healthcaredp

#endif //_SHOCK_MODIFY_STRATEGY_H_
