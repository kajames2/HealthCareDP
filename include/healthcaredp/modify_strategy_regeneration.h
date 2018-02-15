#ifndef _MODIFY_STRATEGY_REGENERATION_H_
#define _MODIFY_STRATEGY_REGENERATION_H_

#include "health_state.h"
#include "modify_strategy.h"
#include "dp_state.h"
#include "regeneration.h"

#include <memory>

namespace healthcaredp {

template <class T>
class ModifyStrategyRegeneration : public genericdp::ModifyStrategy<T> {
 public:
  explicit ModifyStrategyRegeneration(std::shared_ptr<const healthcare::Regeneration> regen);
  void Modify(genericdp::DPState<T>* state) const override;
 private:
  std::shared_ptr<const healthcare::Regeneration> regen_;
};

template <class T>
ModifyStrategyRegeneration<T>::ModifyStrategyRegeneration(
    std::shared_ptr<const healthcare::Regeneration> regen)
    : regen_(regen) {}

template <class T>
void ModifyStrategyRegeneration<T>::Modify(genericdp::DPState<T> *state) const {
  if (IsAlive(state->domain)) {
    state->domain.health += regen_->GetHealthRegained(state->domain.health, state->domain.health_investment);
    state->domain.health = std::min(state->domain.health, 100);
  }
}

}  // namespace healthcaredp

#endif //_MODIFY_STRATEGY_REGENERATION_H_
