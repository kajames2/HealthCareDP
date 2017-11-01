#ifndef _HEALTH_STATE_ITERATOR_H_
#define _HEALTH_STATE_ITERATOR_H_

#include "health_state.h"
#include "state_iterator.h"

#include <iterator>

namespace healthcaredp {
class HealthStateIterator
    : public genericdp::StateIterator<healthcare::HealthState> {
public:
  HealthStateIterator(int max_periods, int max_remaining_cash);
  virtual HealthStateIterator &operator++() override;

 private:
  int max_periods_;
  int max_remaining_cash_;
};
} // namespace healthcaredp
#endif // _HEALTH_STATE_ITERATOR_H_
