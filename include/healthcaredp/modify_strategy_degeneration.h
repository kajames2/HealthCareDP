#ifndef _MODIFY_STRATEGY_DEGENERATION_H_
#define _MODIFY_STRATEGY_DEGENERATION_H_

#include "modify_strategy.h"
#include "dp_state.h"
#include "degeneration.h"

#include <memory>

namespace healthcaredp {
template <class T>
class ModifyStrategyDegeneration : public genericdp::ModifyStrategy<T> {
 public:
  explicit ModifyStrategyDegeneration(std::shared_ptr<const healthcare::Degeneration> degen);
  void Modify(genericdp::DPState<T>* state) const override;
 private:
  std::shared_ptr<const healthcare::Degeneration> degen_;
};

template <class T>
ModifyStrategyDegeneration<T>::ModifyStrategyDegeneration(
    std::shared_ptr<const healthcare::Degeneration> degen)
    : degen_(degen) {}

template <class T>
void ModifyStrategyDegeneration<T>::Modify(genericdp::DPState<T> *state) const {
  state->domain.health -= degen_->GetDegeneration(state->domain.period);
}

}  // namespace healthcaredp

#endif //_MODIFY_STRATEGY_DEGENERATION_H_
