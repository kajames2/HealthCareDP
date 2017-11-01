#ifndef _HEALTH_STATE_TO_EXOGENOUS_ADAPTER_FACTORY_H_
#define _HEALTH_STATE_TO_EXOGENOUS_ADAPTER_FACTORY_H_

#include "degeneration.h"
#include "exogenous_factory.h"
#include "harvest.h"
#include "health_investment_state_factory.h"
#include "health_state_to_exogenous_adapter.h"
#include <memory>

namespace healthcaredp {
class HealthStateToExogenousAdapterFactory
    : public genericdp::ExogenousFactory<healthcare::HealthState> {
public:
  explicit HealthStateToExogenousAdapterFactory(
      healthcare::HealthInvestmentStateFactory investment_fact);
  HealthStateToExogenousAdapterFactory(
      std::shared_ptr<const healthcare::Harvest> harvest,
      std::shared_ptr<const healthcare::Degeneration> degen);
  virtual std::unique_ptr<genericdp::ExogenousState<healthcare::HealthState>>
  GetExogenous(const healthcare::HealthState &state) const override;

private:
  healthcare::HealthInvestmentStateFactory investment_fact_;
};
} // namespace healthcaredp
#endif // _HEALTH_STATE_TO_EXOGENOUS_ADAPTER_FACTORY_H_
