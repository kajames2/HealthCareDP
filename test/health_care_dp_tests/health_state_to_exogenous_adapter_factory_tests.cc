#include "health_state.h"
#include "degeneration.h"
#include "harvest.h"
#include "health_state_to_exogenous_adapter_factory.h"

#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <memory>

class HealthStateToExogenousAdapterFactoryTest : public ::testing::Test {
public:
  HealthStateToExogenousAdapterFactoryTest() {}

protected:
  class MockDegeneration : public healthcare::Degeneration {
  public:
    MockDegeneration() {}
    virtual int GetDegeneration(int period) const override {
      return 5 * period;
    }
  };

  class MockHarvest : public healthcare::Harvest {
  public:
    MockHarvest() {}
    virtual int
    CalculateHarvest(const healthcare::HealthState &state) const override {
      return state.period * 40;
    }
    virtual bool InRange(int period) const override { return true; }
  };

  virtual void SetUp() {
    state_ = healthcare::HealthState(1, 50, 45, 10);
    std::shared_ptr<const MockHarvest> harvest =
        std::make_shared<const MockHarvest>();
    std::shared_ptr<const MockDegeneration> degen =
        std::make_shared<const MockDegeneration>();
    fact_ = std::make_unique<healthcare::HealthInvestmentStateFactory>(harvest,
                                                                       degen);
    ex_factory_ =
        std::make_unique<healthcaredp::HealthStateToExogenousAdapterFactory>(
            *fact_);
    ex_factory_short_ =
        std::make_unique<healthcaredp::HealthStateToExogenousAdapterFactory>(
            harvest, degen);
  }
  healthcare::HealthState state_;
  std::unique_ptr<healthcare::HealthInvestmentStateFactory> fact_;
  std::unique_ptr<healthcaredp::HealthStateToExogenousAdapterFactory>
      ex_factory_;
  std::unique_ptr<healthcaredp::HealthStateToExogenousAdapterFactory>
      ex_factory_short_;
};

TEST_F(HealthStateToExogenousAdapterFactoryTest, EquivalenceTest) {
  healthcare::HealthState health_state_ref = fact_->GetInvestmentState(state_);
  std::shared_ptr<genericdp::ExogenousState<healthcare::HealthState>>
      ex_state_1 = ex_factory_->GetExogenous(state_);
  std::shared_ptr<genericdp::ExogenousState<healthcare::HealthState>>
      ex_state_2 = ex_factory_short_->GetExogenous(state_);
  ASSERT_EQ(health_state_ref, ex_state_1->GetState());
  ASSERT_EQ(health_state_ref, ex_state_2->GetState());
}
