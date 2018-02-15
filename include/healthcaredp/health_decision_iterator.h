#ifndef _HEALTH_DECISION_ITERATOR_H_
#define _HEALTH_DECISION_ITERATOR_H_

#include "health_state.h"
#include "dp_state.h"
#include "dp_state_iterator.h"

#include <algorithm>
#include <memory>

namespace healthcaredp {
template <class T>
class HealthDecisionIterator : public genericdp::DPStateIterator<T> {
public:
  HealthDecisionIterator(const genericdp::DPState<T> &state, int max_remaining_cash);

protected:
  virtual void NextTemplate() override;

private:
  int RemainingCash() const;
  int max_remaining_cash_;
};

template <class T>
HealthDecisionIterator<T>::HealthDecisionIterator(const genericdp::DPState<T> &state,
                                                  int max_remaining_cash)
    : genericdp::DPStateIterator<T>(state),
      max_remaining_cash_(max_remaining_cash) {
  this->template_state_.domain.investment_cash = state.domain.cash;
  this->template_state_.domain.investment_health = state.domain.health;
  if (!IsAlive(this->template_state_.domain)) {
    this->template_state_.domain.life_investment = 0;
    this->template_state_.domain.health_investment = 0;
    this->done_ = true;
  } else {
    this->template_state_.domain.life_investment = std::max(
        0, this->template_state_.domain.investment_cash - max_remaining_cash);
    this->template_state_.domain.health_investment = 0;
    this->template_state_.domain.cash = RemainingCash();
  }
  this->SetState();
}

template <class T> void HealthDecisionIterator<T>::NextTemplate() {
  if (RemainingCash() > 0) {
    this->template_state_.domain.life_investment += 1;
  } else if (this->template_state_.domain.health_investment <
             this->template_state_.domain.investment_cash) {
    this->template_state_.domain.health_investment += 1;
    this->template_state_.domain.life_investment =
        std::max(0, this->template_state_.domain.investment_cash -
                        this->template_state_.domain.health_investment -
                        max_remaining_cash_);
  } else {
    this->done_ = true;
  }
  this->template_state_.domain.cash = RemainingCash();
}

template <class T> int HealthDecisionIterator<T>::RemainingCash() const {
  return this->template_state_.domain.investment_cash -
         this->template_state_.domain.life_investment -
         this->template_state_.domain.health_investment;
}

} // namespace genericdp
#endif // _HEALTH_DECISION_ITERATOR_H_
