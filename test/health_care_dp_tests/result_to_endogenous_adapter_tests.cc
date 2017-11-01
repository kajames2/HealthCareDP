#include "result_to_endogenous_adapter.h"
#include "endogenous_state.h"
#include "health_state.h"
#include "investment.h"
#include "period_result.h"

#include <gtest/gtest.h>
#include <memory>

class ResultToEndogenousAdapterTest : public ::testing::Test {
public:
  ResultToEndogenousAdapterTest() {}

protected:
  virtual void SetUp() {
    state_ = healthcare::HealthState(1, 50, 45, 10);
    healthcare::Investment investment(20, 10);
    life_enjoyment_ = 50;
    healthcare::PeriodResult result(state_, investment, life_enjoyment_);
    end_state_ =
        std::make_unique<healthcaredp::ResultToEndogenousAdapter>(result);
  }
  healthcare::HealthState state_;
  int life_enjoyment_;
  std::unique_ptr<healthcaredp::ResultToEndogenousAdapter> end_state_;
};

TEST_F(ResultToEndogenousAdapterTest, GetStateTest) {
  ASSERT_EQ(state_, end_state_->GetState());
}

TEST_F(ResultToEndogenousAdapterTest, GetValueTest) {
  ASSERT_EQ(life_enjoyment_, end_state_->GetValue());
}

TEST_F(ResultToEndogenousAdapterTest, CloneTest) {
  auto copy = end_state_->Clone();
  ASSERT_EQ(end_state_->GetState(), copy->GetState());
  ASSERT_EQ(end_state_->GetValue(), copy->GetValue());
}

TEST_F(ResultToEndogenousAdapterTest, UpdateEndStateTest) {
  end_state_->UpdateEndState(15, 30, 45);
  ASSERT_EQ(45, end_state_->GetValue());
  ASSERT_EQ(healthcare::HealthState(1, 15, 30, 10), end_state_->GetState());
}
