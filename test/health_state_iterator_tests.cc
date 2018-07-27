#include "health_state.h"
#include "health_state_iterator.h"

#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <memory>

class HealthStateIteratorTest : public ::testing::Test {
public:
  HealthStateIteratorTest() {}

protected:
  virtual void SetUp() {
    max_periods_ = 10;
    max_remaining_cash_ = 15;
    state_it_ = std::make_unique<healthcaredp::HealthStateIterator>(max_periods_, max_remaining_cash_);
  }

  int max_periods_;
  int max_remaining_cash_;
  std::unique_ptr<healthcaredp::HealthStateIterator> state_it_;
};

TEST_F(HealthStateIteratorTest, CorrectNumIts) {
  int count = 0;
  while (*state_it_) {
    ++*state_it_;
    ++count;
  }

  int expected_count = (max_periods_) * (max_remaining_cash_ + 1) * (100 + 1);
  ASSERT_EQ(expected_count, count);
}
