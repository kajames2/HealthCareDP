#ifndef _RESULT_TO_ENDOGENOUS_ADAPTER_H_
#define _RESULT_TO_ENDOGENOUS_ADAPTER_H_

#include "endogenous_state.h"
#include "health_state.h"
#include "investment.h"
#include "period_result.h"

namespace healthcaredp {
class ResultToEndogenousAdapter
    : public genericdp::EndogenousState<healthcare::HealthState> {
public:
  explicit ResultToEndogenousAdapter(healthcare::PeriodResult end_state);
  healthcare::HealthState GetState() const override;
  double GetValue() const override;
  std::unique_ptr<genericdp::EndogenousState<healthcare::HealthState>>
  Clone() const override;
  std::string GetString() const override;
  std::string GetHeader() const override;
  void SetInvestment(healthcare::Investment investment);
  void UpdateEndState(int end_health, int end_cash, double life_enjoyment);

private:
  healthcare::PeriodResult end_state_;
};
} // namespace healthcaredp
#endif // _RESULT_TO_ENDOGENOUS_ADAPTER_H_
