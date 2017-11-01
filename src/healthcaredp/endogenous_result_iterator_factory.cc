#include "endogenous_result_iterator_factory.h"
#include "exogenous_state.h"
#include "health_state.h"
#include "period_result_factory.h"

#include <memory>

namespace healthcaredp {
EndogenousResultIteratorFactory::EndogenousResultIteratorFactory(
    std::shared_ptr<const healthcare::Regeneration> regen,
    std::shared_ptr<const healthcare::Consumption> consumption,
    int max_remaining_cash)
    : state_factory_(regen, consumption),
      max_remaining_cash_(max_remaining_cash) {}

EndogenousResultIteratorFactory::EndogenousResultIteratorFactory(
    const healthcare::PeriodResultFactory &state_factory,
    int max_remaining_cash)
    : state_factory_(state_factory), max_remaining_cash_(max_remaining_cash) {}

std::unique_ptr<genericdp::EndogenousIterator<healthcare::HealthState>>
EndogenousResultIteratorFactory::GetIterator(
    const genericdp::ExogenousState<healthcare::HealthState> &state) const {
  return std::make_unique<EndogenousResultIterator>(state_factory_, state.GetState(),
                                  max_remaining_cash_);
}
} // namespace healthcaredp
