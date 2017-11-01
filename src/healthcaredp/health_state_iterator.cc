#include "health_state_iterator.h"

namespace healthcaredp {
HealthStateIterator::HealthStateIterator(int max_periods,
                                         int max_remaining_cash)
    : max_periods_(max_periods)
    , max_remaining_cash_(max_remaining_cash) {
  state_ = healthcare::HealthState(max_periods, 0, 0, 0);
}

HealthStateIterator &HealthStateIterator::operator++() {
  if (state_.cash < max_remaining_cash_) {
    ++state_.cash;
    return *this;
  }
  state_.cash = 0;

  if (state_.health < 100) {
    ++state_.health;
    return *this;
  }
  state_.health = 0;

  if (state_.period > 1) {
    --state_.period;
    return *this;
  }

  done_ = true;
  return *this;
}
}
