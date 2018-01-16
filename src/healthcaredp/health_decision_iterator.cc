#include "health_decision_iterator.h"

namespace healthcaredp {

HealthDecisionIterator::HealthDecisionIterator(const DPHealthState &state,
                                               int max_remaining_cash)
    : genericdp::DPStateIterator<DPHealthState>(state),
      max_remaining_cash_(max_remaining_cash) {
  template_state_.investment_cash = state.cash;
  template_state_.investment_health = state.health;
  if (!IsAlive(template_state_)) {
    template_state_.life_investment = 0;
    template_state_.health_investment = 0;
    done_ = true;
  } else {
    template_state_.life_investment = std::max(0, template_state_.investment_cash - max_remaining_cash);
    template_state_.health_investment = 0;
    template_state_.cash = RemainingCash();
  }
  this->SetState();
}

void HealthDecisionIterator::NextTemplate() {
  if (RemainingCash() > 0) {
    template_state_.life_investment += 1;
  } else if (template_state_.health_investment <
             template_state_.investment_cash) {
    template_state_.health_investment += 1;
    template_state_.life_investment = std::max(
        0, template_state_.investment_cash - template_state_.health_investment -
               max_remaining_cash_);
  } else {
    done_ = true;
  }
  template_state_.cash = RemainingCash();
}

int HealthDecisionIterator::RemainingCash() const {
  return template_state_.investment_cash - template_state_.life_investment -
         template_state_.health_investment;
}

} // namespace healthcaredp
