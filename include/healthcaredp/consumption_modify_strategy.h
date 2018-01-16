#ifndef _CONSUMPTION_MODIFY_STRATEGY_H_
#define _CONSUMPTION_MODIFY_STRATEGY_H_

#include "modify_strategy.h"
#include "dp_health_state.h"
#include "consumption.h"
#include <memory>

namespace healthcaredp {
class ConsumptionModifyStrategy : public genericdp::ModifyStrategy<DPHealthState> {
 public:
  explicit ConsumptionModifyStrategy(std::shared_ptr<const healthcare::Consumption> consumption);
  void Modify(DPHealthState* state) const override;
 private:
  std::shared_ptr<const healthcare::Consumption> consumption_;
};

}  // namespace healthcaredp

#endif //_CONSUMPTION_MODIFY_STRATEGY_H_
