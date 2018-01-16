#ifndef _REGENERATION_MODIFY_STRATEGY_H_
#define _REGENERATION_MODIFY_STRATEGY_H_

#include "modify_strategy.h"
#include "dp_health_state.h"
#include "regeneration.h"

#include <memory>

namespace healthcaredp {
class RegenerationModifyStrategy : public genericdp::ModifyStrategy<DPHealthState> {
 public:
  explicit RegenerationModifyStrategy(std::shared_ptr<const healthcare::Regeneration> regen);
  void Modify(DPHealthState* state) const override;
 private:
  std::shared_ptr<const healthcare::Regeneration> regen_;
};

}  // namespace healthcaredp

#endif //_REGENERATION_MODIFY_STRATEGY_H_
