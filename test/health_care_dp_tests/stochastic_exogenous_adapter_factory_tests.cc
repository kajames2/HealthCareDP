#include "stochastic_exogenous_adapter_factory.h"
#include "exogenous_factory.h"
#include "exogenous_state.h"
#include "health_state.h"
#include "health_state_to_exogenous_adapter.h"

#include <gtest/gtest.h>
#include <memory>

class StochasticExogenousAdapterFactoryTests : public ::testing::Test {
public:
  StochasticExogenousAdapterFactoryTests() {}

protected:
  class MockStateFactory
      : public genericdp::ExogenousFactory<healthcare::HealthState> {
    using HealthExogenous = healthcaredp::HealthStateToExogenousAdapter;

  public:
    MockStateFactory() {}
    std::unique_ptr<genericdp::ExogenousState<healthcare::HealthState>>
    GetExogenous(const healthcare::HealthState &state) const override {
      return std::make_unique<HealthExogenous>(healthcare::HealthState(
          state.period, state.health - 20, state.cash + 20,
          state.total_working_harvest));
    }
  };

  virtual void SetUp() {
    prob_ = 0.3;
    state_ = healthcare::HealthState(1, 50, 45, 10);
    auto ex_factory = std::make_unique<MockStateFactory>();
    stoch_factory_ = healthcaredp::StochasticExogenousAdapterFactory(
        std::move(ex_factory), prob_);
  }

  healthcaredp::StochasticExogenousAdapterFactory stoch_factory_;
  healthcare::HealthState state_;
  double prob_;
};

TEST_F(StochasticExogenousAdapterFactoryTests, CreatesCorrectlyTest) {
  auto stoch_state = stoch_factory_.GetStochasticExogenous(state_);
  ASSERT_EQ(prob_, stoch_state.GetProbability());
  ASSERT_EQ(healthcare::HealthState(1, 30, 65, 10),
            stoch_state.GetExogenousState().GetState());
}
