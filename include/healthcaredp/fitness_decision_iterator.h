#ifndef _FITNESS_DECISION_ITERATOR_H_
#define _FITNESS_DECISION_ITERATOR_H_

#include "dp_health_state.h"
#include "dp_state_iterator.h"

#include <memory>

namespace healthcaredp {
template <class T>
class FitnessDecisionIterator
    : public genericdp::DPStateIterator<T> {
public:
  FitnessDecisionIterator(const T &state);

protected:
  virtual void NextTemplate() override;

private:
};
} // namespace genericdp
#endif // _FITNESS_DECISION_ITERATOR_H_
