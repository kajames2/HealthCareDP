#ifndef _HEALTH_DECISION_ITERATOR_FACTORY_H_
#define _HEALTH_DECISION_ITERATOR_FACTORY_H_

#include "dp_state.h"
#include "dp_state_iterator_factory.h"
#include "health_decision_iterator.h"

#include <memory>

namespace healthcaredp {
template <class T>
class HealthDecisionIteratorFactory
    : public genericdp::DPStateIteratorFactory<T> {
public:
  HealthDecisionIteratorFactory(int max_remaining_cash);
  std::unique_ptr<genericdp::DPStateIterator<T>>
  GetIterator(const genericdp::DPState<T> &input) const override;

private:
  int max_remaining_cash_;
};

template <class T>
HealthDecisionIteratorFactory<T>::HealthDecisionIteratorFactory(
    int max_remaining_cash)
    : max_remaining_cash_(max_remaining_cash) {}

template <class T>
std::unique_ptr<genericdp::DPStateIterator<T>>
HealthDecisionIteratorFactory<T>::GetIterator(const genericdp::DPState<T> &input) const {
  return std::make_unique<HealthDecisionIterator<T>>(input,
                                                     max_remaining_cash_);
}

} // namespace healthcaredp
#endif // _HEALTH_DECISION_ITERATOR_FACTORY_H_
