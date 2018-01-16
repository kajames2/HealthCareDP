#ifndef _HARVEST_MODIFY_STRATEGY_H_
#define _HARVEST_MODIFY_STRATEGY_H_

#include "modify_strategy.h"
#include "dp_health_state.h"
#include "harvest.h"

#include <memory>

namespace healthcaredp {
class HarvestModifyStrategy : public genericdp::ModifyStrategy<DPHealthState> {
 public:
  explicit HarvestModifyStrategy(std::shared_ptr<const healthcare::Harvest> harvest);
  void Modify(DPHealthState* state) const override;
 private:
  std::shared_ptr<const healthcare::Harvest> harvest_;
};

}  // namespace healthcaredp

#endif //_HARVEST_MODIFY_STRATEGY_H_
