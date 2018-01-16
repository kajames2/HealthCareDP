#ifndef _DEGENERATION_MODIFY_STRATEGY_H_
#define _DEGENERATION_MODIFY_STRATEGY_H_

#include "modify_strategy.h"
#include "dp_health_state.h"
#include "degeneration.h"

#include <memory>

namespace healthcaredp {
class DegenerationModifyStrategy : public genericdp::ModifyStrategy<DPHealthState> {
 public:
  explicit DegenerationModifyStrategy(std::shared_ptr<const healthcare::Degeneration> degen);
  void Modify(DPHealthState* state) const override;
 private:
  std::shared_ptr<const healthcare::Degeneration> degen_;
};

}  // namespace healthcaredp

#endif //_DEGENERATION_MODIFY_STRATEGY_H_
