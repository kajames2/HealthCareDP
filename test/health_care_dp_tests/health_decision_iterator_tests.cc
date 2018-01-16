#include "health_state.h"
#include "health_decision_iterator.h"
#include "dp_health_state.h"

#include <gtest/gtest.h>
#include <memory>

class HealthDecisionIteratorTest : public ::testing::Test {
public:
  HealthDecisionIteratorTest() {}

protected:
  virtual void SetUp() {
    max_remaining_cash_ = 15;
    state_ = healthcaredp::DPHealthState(healthcare::HealthState(1, 50, 50, 10));
    end_it_ = std::make_unique<healthcaredp::HealthDecisionIterator>(state_, max_remaining_cash_);
    state2_ = healthcaredp::DPHealthState(healthcare::HealthState(1, 50, 0, 0));
    end_it2_ = std::make_unique<healthcaredp::HealthDecisionIterator>(state2_, max_remaining_cash_);
    
  }

  int max_remaining_cash_;
  healthcaredp::DPHealthState state_, state2_;
  std::unique_ptr<healthcaredp::HealthDecisionIterator> end_it_, end_it2_;
};

TEST_F(HealthDecisionIteratorTest, CorrectNumIts) {
  int count = 0;
  while (*end_it_) {
    ++*end_it_;
    ++count;
  }

  int expected_count = (52*51)/2 - (36*35)/2;
  ASSERT_EQ(expected_count, count);
}
