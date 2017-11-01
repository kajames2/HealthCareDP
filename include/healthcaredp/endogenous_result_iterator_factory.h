#ifndef _ENDOGENOUS_RESULT_ITERATOR_FACTORY_H_
#define _ENDOGENOUS_RESULT_ITERATOR_FACTORY_H_

#include "consumption.h"
#include "endogenous_iterator_factory.h"
#include "endogenous_result_iterator.h"
#include "health_state.h"
#include "health_state_to_exogenous_adapter.h"
#include "regeneration.h"
#include "period_result_factory.h"

#include <memory>

namespace healthcaredp {
class EndogenousResultIteratorFactory
    : public genericdp::EndogenousIteratorFactory<healthcare::HealthState> {
public:
  using EndIt = genericdp::EndogenousIterator<healthcare::HealthState>;
  using ExState = genericdp::ExogenousState<healthcare::HealthState>;
  
  EndogenousResultIteratorFactory(
      std::shared_ptr<const healthcare::Regeneration> regen,
      std::shared_ptr<const healthcare::Consumption> consumption,
      int max_cash_on_hand = 0);
  EndogenousResultIteratorFactory(
      const healthcare::PeriodResultFactory &state_factory,
      int max_remaining_cash = 0);
  virtual std::unique_ptr<EndIt>
  GetIterator(const ExState &state) const override;

private:
  healthcare::PeriodResultFactory state_factory_;
  int max_remaining_cash_;
};
} // namespace healthcaredp
#endif // _ENDOGENOUS_RESULT_ITERATOR_FACTORY_H_
