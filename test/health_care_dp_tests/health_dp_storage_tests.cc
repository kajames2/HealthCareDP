#include "health_dp_storage.h"
#include "health_state.h"
#include "dp_health_state.h"
#include "dp_result.h"

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
    state = healthcaredp::DPHealthState(healthcare::HealthState(1, 50, 15, 10));
    state2 = healthcaredp::DPHealthState(healthcare::HealthState(2, 100, 0, 10));
    state_dead = healthcaredp::DPHealthState(healthcare::HealthState(2, 0, 0, 0));
    state_end = healthcaredp::DPHealthState(healthcare::HealthState(4, 50, 30, 10));
    state_end.value = 50;
    state_invalid = healthcaredp::DPHealthState(healthcare::HealthState(2, 100, 60, 0));
    res.AddState(state_end);
    storage = std::make_unique<healthcaredp::HealthDPStorage>(3, 50);
  }

  int max_remaining_cash_;
  healthcaredp::DPHealthState state, state2, state_bad, state_dead, state_end, state_invalid;
  std::unique_ptr<healthcaredp::HealthDPStorage> storage;
  genericdp::DPResult<healthcaredp::DPHealthState> res;
};

TEST_F(HealthDPStorageTest, StoreAndCheckTest) {
  ASSERT_FALSE(storage->IsStoredState(state));
  storage->StoreOptimalResult(state, res);
  ASSERT_TRUE(storage->IsStoredState(state));
}

TEST_F(HealthDPStorageTest, StoreAndRetrieveValueTest) {
  storage->StoreOptimalResult(state, res);
  ASSERT_DOUBLE_EQ(50, storage->GetOptimalValue(state));
}

TEST_F(HealthDPStorageTest, StoreAndRetrieveStateTest) {
  storage->StoreOptimalResult(state, res);
  auto stored_res = storage->GetOptimalResult(state);
  ASSERT_EQ(res[0].cash, stored_res[0].cash);
}

TEST_F(HealthDPStorageTest, IsTerminalStateTest) {
  ASSERT_TRUE(storage->IsTerminalState(state_dead));
  ASSERT_TRUE(storage->IsTerminalState(state_end));
  ASSERT_FALSE(storage->IsTerminalState(state_invalid));
  ASSERT_FALSE(storage->IsTerminalState(state));
}

TEST_F(HealthDPStorageTest, ThrowsForInvalidRetrievalTest) {
  ASSERT_THROW(storage->StoreOptimalResult(state_invalid, res), std::out_of_range);
}

TEST_F(HealthDPStorageTest, ReturnsDefaultConstructedIfUnstoredTest) {
  auto default_res = storage->GetOptimalResult(state);
  ASSERT_EQ(default_res.GetStates().size(), 0);
  ASSERT_DOUBLE_EQ(0, default_res.GetValue());
}
