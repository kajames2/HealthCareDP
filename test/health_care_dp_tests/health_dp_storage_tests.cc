#include "health_dp_storage.h"
#include "health_state.h"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <stdexcept>

class HealthDPStorageTest : public ::testing::Test {
public:
  HealthDPStorageTest() {}

protected:
  virtual void SetUp() {
    max_remaining_cash_ = 15;
     state = healthcare::HealthState(1, 50, 20, 10);
     state2 = healthcare::HealthState(2, 100, 50, 10);
     state_dead = healthcare::HealthState(2, 0, 0, 0);
     state_end = healthcare::HealthState(4, 50, 30, 10);
     state_invalid = healthcare::HealthState(2, 100, 60, 0);
     storage = std::make_unique<healthcaredp::HealthDPStorage<std::string>>(3,50);
  }

  int max_remaining_cash_;
  healthcare::HealthState state, state2, state_bad, state_dead, state_end, state_invalid;
  std::unique_ptr<healthcaredp::HealthDPStorage<std::string>> storage;
};

TEST_F(HealthDPStorageTest, StoreAndCheckTest) {
  ASSERT_FALSE(storage->IsStoredState(state));
  storage->StoreOptimalResult(state, "opt_state", 5);
  ASSERT_TRUE(storage->IsStoredState(state));
}

TEST_F(HealthDPStorageTest, StoreAndRetrieveValueTest) {
  storage->StoreOptimalResult(state, "opt_state", 5.3);
  ASSERT_DOUBLE_EQ(5.3, storage->GetOptimalValue(state));
}

TEST_F(HealthDPStorageTest, StoreAndRetrieveStateTest) {
  storage->StoreOptimalResult(state, "opt_state",5.3);
  ASSERT_EQ(0, storage->GetOptimalResult(state).compare("opt_state"));
}

TEST_F(HealthDPStorageTest, IsTerminalStateTest) {
  ASSERT_TRUE(storage->IsTerminalState(state_dead));
  ASSERT_TRUE(storage->IsTerminalState(state_end));
  ASSERT_FALSE(storage->IsTerminalState(state_invalid));
  ASSERT_FALSE(storage->IsTerminalState(state));
}

TEST_F(HealthDPStorageTest, ThrowsForInvalidRetrievalTest) {
  ASSERT_THROW(storage->StoreOptimalResult(state_invalid, "opt", 5.3), std::out_of_range);
}

TEST_F(HealthDPStorageTest, ReturnsDefaultConstructedIfUnstoredTest) {
  ASSERT_EQ(storage->GetOptimalResult(state).compare(""), 0);
}
