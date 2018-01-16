#ifndef _HEALTH_DP_RETIREMENT_STORAGE_H_
#define _HEALTH_DP_RETIREMENT_STORAGE_H_

#include "dp_health_state.h"
#include "single_vector_dp_storage.h"

#include <iostream>

namespace healthcaredp {
class HealthDPRetirementStorage
    : public genericdp::SingleVectorDPStorage<DPHealthState> {
public:
  HealthDPRetirementStorage(int max_periods, int max_remaining_cash,
                            int max_working_harvest);
  bool IsTerminalState(const DPHealthState &state) const override;

protected:
  virtual bool IsValidState(const DPHealthState &state) const override;
  virtual int GetIndex(const DPHealthState &state) const override;

private:
  int max_periods_;
  int max_remaining_cash_;
  int max_working_harvest_;
  const int period_table_size_;
  const int period_health_table_size_;
  const int period_health_cash_row_size_;
};

HealthDPRetirementStorage::HealthDPRetirementStorage(
    int max_periods, int max_remaining_cash, int max_working_harvest)
    : max_periods_(max_periods)
    , max_remaining_cash_(max_remaining_cash)
    , max_working_harvest_(max_working_harvest)
    , period_table_size_((100 + 1) * (max_remaining_cash + 1) *
                         (max_working_harvest + 1))
    , period_health_table_size_((max_remaining_cash + 1) *
                                (max_working_harvest + 1))
    , period_health_cash_row_size_(max_working_harvest + 1)
    , genericdp::SingleVectorDPStorage<DPHealthState>(
        max_periods * (100 + 1) * (max_remaining_cash + 1) * (max_working_harvest + 1)){
}

bool HealthDPRetirementStorage::IsTerminalState(
    const DPHealthState &state) const {
  return state.period > max_periods_ || !healthcare::IsAlive(state);
}

int HealthDPRetirementStorage::GetIndex(
    const DPHealthState &state) const {
  if (IsValidState(state)) {
    return ((state.period - 1) * period_table_size_) +
           (state.health * period_health_table_size_) +
           (state.cash * period_health_cash_row_size_) +
           state.total_working_harvest;
  } else {
    throw std::out_of_range("HealthState out of range.");
  }
}

bool HealthDPRetirementStorage::IsValidState(
    const DPHealthState &state) const {
  return state.period > 0 && state.period <= max_periods_ &&
         state.health >= 0 && state.health <= 100 && state.cash >= 0 &&
         state.cash <= max_remaining_cash_ &&
         state.total_working_harvest <= max_working_harvest_;
}
} // namespace healthcaredp
#endif // _HEALTH_DP_RETIREMENT_STORAGE_H_
