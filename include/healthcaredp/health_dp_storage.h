#ifndef _HEALTH_DP_STORAGE_H_
#define _HEALTH_DP_STORAGE_H_

#include "health_state.h"
#include "dp_storage_single_vector.h"
#include <iostream>

namespace healthcaredp {
template <class T>
class HealthDPStorage : public genericdp::DPStorageSingleVector<T> {
public:
  HealthDPStorage(int max_periods, int max_remaining_cash);
  bool IsTerminalState(const T &state) const override;

protected:
  bool IsValidState(const T &state) const override;
  int GetIndex(const T &state) const override;

private:
  int max_periods_;
  int max_remaining_cash_;
  int sub_table_size_;
  int row_size_;
};

template <class T>
HealthDPStorage<T>::HealthDPStorage(int max_periods, int max_remaining_cash)
    : max_periods_(max_periods)
    , max_remaining_cash_(max_remaining_cash)
    , sub_table_size_(100 * (max_remaining_cash_ +1))
    , row_size_(max_remaining_cash_ + 1)
    , genericdp::DPStorageSingleVector<T>(
          max_periods * 100 * (max_remaining_cash + 1)) {}

template <class T>
bool HealthDPStorage<T>::IsTerminalState(const T &state) const {
  return state.period > max_periods_ || !healthcare::IsAlive(state);
}

template <class T>
int HealthDPStorage<T>::GetIndex(const T &state) const {
  if (IsValidState(state)) {
    return ((state.period - 1) * sub_table_size_) + ((state.health - 1) * row_size_) +
           state.cash;
  } else {
    std::cout << state.period << ", " << state.health << ", " << state.cash << std::endl;
    throw std::out_of_range("HealthState out of range.");
  }
}

template <class T>
bool HealthDPStorage<T>::IsValidState(const T &state) const {
  return state.period > 0 && state.period <= max_periods_ &&
         state.health > 0 && state.health <= 100 && state.cash >= 0 &&
         state.cash <= max_remaining_cash_;
}

} // namespace healthcaredp
#endif // _HEALTH_DP_STORAGE_H_
