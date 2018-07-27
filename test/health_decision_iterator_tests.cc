#include <gtest/gtest.h>

#include <memory>

#include "genericdp/dp_state.h"
#include "healthcare/health_state.h"
#include "healthcaredp/health_decision_iterator.h"

class HealthDecisionIteratorTest : public ::testing::Test {
 public:
  HealthDecisionIteratorTest() {}

 protected:
  virtual void SetUp() {
    max_remaining_cash_ = 15;
    state_ = genericdp::DPState<healthcare::HealthState>(
        healthcare::HealthState(1, 50, 50, 10));
    end_it_ = std::make_unique<
        healthcaredp::HealthDecisionIterator<healthcare::HealthState>>(
        state_, max_remaining_cash_);
    state2_ = genericdp::DPState<healthcare::HealthState>(
        healthcare::HealthState(1, 50, 0, 0));
    end_it2_ = std::make_unique<
        healthcaredp::HealthDecisionIterator<healthcare::HealthState>>(
        state2_, max_remaining_cash_);
  }

  int max_remaining_cash_;
  genericdp::DPState<healthcare::HealthState> state_, state2_;
  std::unique_ptr<healthcaredp::HealthDecisionIterator<healthcare::HealthState>>
      end_it_, end_it2_;
};

TEST_F(HealthDecisionIteratorTest, CorrectNumIts) {
  int count = 0;
  while (*end_it_) {
    ++*end_it_;
    ++count;
  }

  int expected_count = (52 * 51) / 2 - (36 * 35) / 2;
  ASSERT_EQ(expected_count, count);
}
