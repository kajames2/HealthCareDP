#ifndef _ENDOGENOUS_RESULT_ITERATOR_H_
#define _ENDOGENOUS_RESULT_ITERATOR_H_

#include "endogenous_iterator.h"
#include "exogenous_state.h"
#include "health_state.h"
#include "investment.h"
#include "result_to_endogenous_adapter.h"
#include "period_result_factory.h"

#include <memory>

namespace healthcaredp {
class EndogenousResultIterator
    : public genericdp::EndogenousIterator<healthcare::HealthState> {
public:
  EndogenousResultIterator(
      const healthcare::PeriodResultFactory& state_factory,
      const healthcare::HealthState &state,
      int max_remaining_cash = 0);
  virtual EndogenousResultIterator &operator++() override;
private:
  bool SetNextHealthInvestmentIfAble();
  ResultToEndogenousAdapter* GetEndogenousResult();
  bool HasRemainingCash() const;
  int GetHealthRegained() const;
  double GetLifeEnjoyment() const;
  void ChangeHealthInvestment(int health_investment);
  
  const int max_remaining_cash_;
  const healthcare::HealthState invest_state_;
  healthcare::Investment cur_investment_;
  int cur_health_regained_;
  int prev_health_regained_;
  healthcare::PeriodResultFactory state_factory_;
  std::unique_ptr<ResultToEndogenousAdapter> cached_result_;
};
} // namespace healthcaredp
#endif // _ENDOGENOUS_RESULT_ITERATOR_H_
