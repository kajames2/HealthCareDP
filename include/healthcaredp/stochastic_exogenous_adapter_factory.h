#ifndef _STOCHASTIC_EXOGENOUS_ADAPTER_FACTORY_H_
#define _STOCHASTIC_EXOGENOUS_ADAPTER_FACTORY_H_

#include "exogenous_state.h"
#include "health_state.h"
#include "health_state_to_exogenous_adapter_factory.h"
#include "stochastic_exogenous_state.h"
#include <memory>

namespace healthcaredp {
class StochasticExogenousAdapterFactory {
public:
  using StochasticInvestmentState =
      genericdp::StochasticExogenousState<healthcare::HealthState>;
  StochasticExogenousAdapterFactory() = default;
  StochasticExogenousAdapterFactory(
      std::unique_ptr<const genericdp::ExogenousFactory<healthcare::HealthState>> fact,
      double prob);
  StochasticInvestmentState
  GetStochasticExogenous(const healthcare::HealthState &state) const;

private:
  std::unique_ptr<const genericdp::ExogenousFactory<healthcare::HealthState>> fact_;
  double probability_;
};
} // namespace healthcare
#endif //  _STOCHASTIC_EXOGENOUS_ADAPTER_FACTORY_H_
