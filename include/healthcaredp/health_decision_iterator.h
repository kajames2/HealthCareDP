#ifndef _HEALTH_DECISION_ITERATOR_H_
#define _HEALTH_DECISION_ITERATOR_H_

#include "dp_health_state.h"
#include "dp_state_iterator.h"

#include <memory>

namespace healthcaredp {
class HealthDecisionIterator
    : public genericdp::DPStateIterator<DPHealthState> {
public:
  HealthDecisionIterator(const DPHealthState &state, int max_remaining_cash);

protected:
  virtual void NextTemplate() override;

private:
  int RemainingCash() const;
  int max_remaining_cash_;
};

} // namespace genericdp
#endif // _HEALTH_DECISION_ITERATOR_H_
