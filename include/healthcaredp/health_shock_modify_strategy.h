#ifndef _HEALTH_SHOCK_MODIFY_STRATEGY_H_
#define _HEALTH_SHOCK_MODIFY_STRATEGY_H_

#include "modify_strategy.h"
#include "dp_health_state.h"
#include "consumption.h"
#include <memory>

namespace healthcaredp {
class HealthShockModifyStrategy : public genericdp::ModifyStrategy<DPHealthState> {
 public:
  explicit HealthShockModifyStrategy(double prob, int shock_size);
  void Modify(DPHealthState* state) const override;
 private:
  int shock_size_;
  double prob_;
};

}  // namespace healthcaredp

#endif //_HEALTH_SHOCK_MODIFY_STRATEGY_H_
