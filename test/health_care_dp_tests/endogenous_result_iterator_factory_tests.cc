#include "endogenous_result_iterator_factory.h"
#include "consumption.h"
#include "endogenous_result_iterator.h"
#include "health_state.h"
#include "health_state_to_exogenous_adapter.h"
#include "investment.h"
#include "period_result_factory.h"
#include "regeneration.h"

#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <memory>

class EndogenousResultIteratorFactoryTest : public ::testing::Test {
public:
  EndogenousResultIteratorFactoryTest() {}

protected:
  class MockConsumption : public healthcare::Consumption {
  public:
    MockConsumption() {}
    virtual double GetLifeEnjoyment(int health, int li) const override {
      return 2 * li + health;
    }
  };

  class MockRegeneration : public healthcare::Regeneration {
  public:
    MockRegeneration() {}
    virtual int GetHealthRegained(int health, int hi) const override {
      return std::floor(hi / 10.);
    }
  };

  virtual void SetUp() {
    max_remaining_cash_ = 15;
    healthcare::HealthState state(1, 50, 50, 10);
    exog_state_ =
        std::make_unique<healthcaredp::HealthStateToExogenousAdapter>(state);
    std::shared_ptr<const MockRegeneration> regen =
        std::make_shared<const MockRegeneration>();
    std::shared_ptr<const MockConsumption> consumption =
        std::make_shared<const MockConsumption>();
    healthcare::PeriodResultFactory fact =
        healthcare::PeriodResultFactory(regen, consumption);
    end_it_factory_ =
        std::make_unique<healthcaredp::EndogenousResultIteratorFactory>(
            fact, max_remaining_cash_);
    end_it_factory_short_ =
        std::make_unique<healthcaredp::EndogenousResultIteratorFactory>(
            regen, consumption, max_remaining_cash_);
  }

  int max_remaining_cash_;
  std::unique_ptr<genericdp::ExogenousState<healthcare::HealthState>>
      exog_state_;
  std::unique_ptr<genericdp::EndogenousIteratorFactory<healthcare::HealthState>>
      end_it_factory_;
  std::unique_ptr<genericdp::EndogenousIteratorFactory<healthcare::HealthState>>
      end_it_factory_short_;
};

TEST_F(EndogenousResultIteratorFactoryTest, CorrectNumIts) {
  auto it = end_it_factory_->GetIterator(*exog_state_);
  auto it_short = end_it_factory_short_->GetIterator(*exog_state_);

  int count = 0;
  while (*it && *it_short) {
    ++*it;
    ++*it_short;
    ++count;
  }

  int expected_count = 0;
  for (int hi = 0; hi <= exog_state_->GetState().cash; hi += 10) {
    expected_count += std::min(exog_state_->GetState().cash - hi + 1,
                               max_remaining_cash_ + 1);
  }
  ASSERT_EQ(false, *it || *it_short);
  ASSERT_EQ(expected_count, count);
}
