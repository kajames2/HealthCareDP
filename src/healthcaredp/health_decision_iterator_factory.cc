#include "health_decision_iterator_factory.h"

namespace healthcaredp {
HealthDecisionIteratorFactory::HealthDecisionIteratorFactory(int max_remaining_cash)
    : max_remaining_cash_(max_remaining_cash) {}

std::unique_ptr<genericdp::DPStateIterator<DPHealthState>>
HealthDecisionIteratorFactory::GetIterator(const DPHealthState &input) const {
  return std::make_unique<HealthDecisionIterator>(input, max_remaining_cash_);
}
} // namespace healthcaredp
