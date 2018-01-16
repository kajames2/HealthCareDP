#ifndef _HEALTH_DECISION_ITERATOR_FACTORY_H_
#define _HEALTH_DECISION_ITERATOR_FACTORY_H_

#include "dp_state_iterator_factory.h"
#include "health_decision_iterator.h"

#include <memory>

namespace healthcaredp {
class HealthDecisionIteratorFactory
    : public genericdp::DPStateIteratorFactory<DPHealthState> {
public:
  HealthDecisionIteratorFactory(int max_remaining_cash);
  std::unique_ptr<genericdp::DPStateIterator<DPHealthState>>
  GetIterator(const DPHealthState &input) const override;
 private:
  int max_remaining_cash_;
};
} // namespace healthcaredp
#endif // _HEALTH_DECISION_ITERATOR_FACTORY_H_
