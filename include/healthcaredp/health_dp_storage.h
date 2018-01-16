#ifndef _HEALTH_DP_STORAGE_H_
#define _HEALTH_DP_STORAGE_H_

#include "dp_health_state.h"
#include "single_vector_dp_storage.h"
#include <iostream>

namespace healthcaredp {
class HealthDPStorage : public genericdp::SingleVectorDPStorage<DPHealthState> {
public:
  HealthDPStorage(int max_periods, int max_remaining_cash);
  bool IsTerminalState(const DPHealthState &state) const override;

protected:
  bool IsValidState(const DPHealthState &state) const override;
  int GetIndex(const DPHealthState &state) const override;

private:
  int max_periods_;
  int max_remaining_cash_;
  int sub_table_size_;
  int row_size_;
};

HealthDPStorage::HealthDPStorage(int max_periods, int max_remaining_cash)
    : max_periods_(max_periods)
    , max_remaining_cash_(max_remaining_cash)
    , sub_table_size_(100 * (max_remaining_cash_ +1))
    , row_size_(max_remaining_cash_ + 1)
    , genericdp::SingleVectorDPStorage<DPHealthState>(
          max_periods * 100 * (max_remaining_cash + 1)) {}

bool HealthDPStorage::IsTerminalState(const DPHealthState &state) const {
  return state.period > max_periods_ || !healthcare::IsAlive(state);
}

int HealthDPStorage::GetIndex(const DPHealthState &state) const {
  if (IsValidState(state)) {
    return ((state.period - 1) * sub_table_size_) + ((state.health - 1) * row_size_) +
           state.cash;
  } else {
    std::cout << state.period << ", " << state.health << ", " << state.cash << std::endl;
    throw std::out_of_range("HealthState out of range.");
  }
}

bool HealthDPStorage::IsValidState(const DPHealthState &state) const {
  return state.period > 0 && state.period <= max_periods_ &&
         state.health > 0 && state.health <= 100 && state.cash >= 0 &&
         state.cash <= max_remaining_cash_;
}

} // namespace healthcaredp
#endif // _HEALTH_DP_STORAGE_H_
