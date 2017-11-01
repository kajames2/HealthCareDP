#include "stochastic_exogenous_adapter_set_factory.h"
#include "exogenous_factory.h"
#include "exogenous_state.h"
#include "health_state.h"
#include "health_state_to_exogenous_adapter.h"
#include "stochastic_exogenous_adapter_factory.h"

#include <gtest/gtest.h>
#include <memory>

class StochasticExogenousAdapterFactorySetTests : public ::testing::Test {
public:
  StochasticExogenousAdapterFactorySetTests() {}

protected:
  class MockStateFactory1
      : public genericdp::ExogenousFactory<healthcare::HealthState> {
    using HealthExogenous = healthcaredp::HealthStateToExogenousAdapter;

  public:
    MockStateFactory1() {}
    std::unique_ptr<genericdp::ExogenousState<healthcare::HealthState>>
    GetExogenous(const healthcare::HealthState &state) const override {
      return std::make_unique<HealthExogenous>(healthcare::HealthState(
          state.period, state.health - 20, state.cash + 20,
          state.total_working_harvest));
    }
  };

  class MockStateFactory2
      : public genericdp::ExogenousFactory<healthcare::HealthState> {
    using HealthExogenous = healthcaredp::HealthStateToExogenousAdapter;

  public:
    MockStateFactory2() {}
    std::unique_ptr<genericdp::ExogenousState<healthcare::HealthState>>
    GetExogenous(const healthcare::HealthState &state) const override {
      return std::make_unique<HealthExogenous>(healthcare::HealthState(
          state.period, state.health - 40, state.cash + 20,
          state.total_working_harvest));
    }
  };

  virtual void SetUp() {
    prob_ = 0.3;
    state_ = healthcare::HealthState(1, 50, 45, 10);
    auto ex_factory = std::make_unique<MockStateFactory1>();
    auto ex_factory2 = std::make_unique<MockStateFactory2>();
    auto stoch_factory = healthcaredp::StochasticExogenousAdapterFactory(
        std::move(ex_factory), prob_);
    auto stoch_factory2 = healthcaredp::StochasticExogenousAdapterFactory(
        std::move(ex_factory2), 1 - prob_);
    std::vector<healthcaredp::StochasticExogenousAdapterFactory> set;
    set.push_back(std::move(stoch_factory));
    set.push_back(std::move(stoch_factory2));
    set_factory_ =
        std::make_unique<healthcaredp::StochasticExogenousAdapterSetFactory>(
            std::move(set));
  }
  std::unique_ptr<healthcaredp::StochasticExogenousAdapterSetFactory>
      set_factory_;
  healthcare::HealthState state_;
  double prob_;
};

TEST_F(StochasticExogenousAdapterFactorySetTests, CreatesCorrectlyTest) {
  auto stoch_set = set_factory_->GetExogenousSet(state_);
  ASSERT_EQ(prob_, stoch_set[0].GetProbability());
  ASSERT_EQ(1-prob_, stoch_set[1].GetProbability());
  ASSERT_EQ(healthcare::HealthState(1,30,65,10), stoch_set[0].GetExogenousState().GetState());
  ASSERT_EQ(healthcare::HealthState(1,10,65,10), stoch_set[1].GetExogenousState().GetState());
}
