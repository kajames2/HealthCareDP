#ifndef _ACCOUNTING_MODIFY_STRATEGY_H_
#define _ACCOUNTING_MODIFY_STRATEGY_H_

#include "modify_strategy.h"
#include "dp_health_state.h"

#include <memory>

namespace healthcaredp {
class AccountingModifyStrategy : public genericdp::ModifyStrategy<DPHealthState> {
 public:
  AccountingModifyStrategy();
  void Modify(DPHealthState* state) const override;
};

}  // namespace healthcaredp

#endif //_ACCOUNTING_MODIFY_STRATEGY_H_
