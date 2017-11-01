#include "exogenous_state.h"
#include "health_state_to_exogenous_adapter.h"
#include "health_state.h"


#include <gtest/gtest.h>
#include <memory>

class HealthStateToExogenousAdapterTest : public ::testing::Test {
public:
  HealthStateToExogenousAdapterTest() {}

protected:
  virtual void SetUp() {
    state_ = healthcare::HealthState(1, 50, 45, 10);
    int_state_ = std::make_unique<healthcaredp::HealthStateToExogenousAdapter>(state_);
  }
  healthcare::HealthState state_;
  int life_enjoyment_;
  std::unique_ptr<genericdp::ExogenousState<healthcare::HealthState>> int_state_;
};

TEST_F(HealthStateToExogenousAdapterTest, GetStateTest) {
  ASSERT_EQ(state_, int_state_->GetState());
}

TEST_F(HealthStateToExogenousAdapterTest, CloneTest) {
  auto clone = int_state_->Clone();
  ASSERT_EQ(clone->GetState(), int_state_->GetState());
}
