#ifndef _HEALTH_DP_STORAGE_H_
#define _HEALTH_DP_STORAGE_H_

#include "dp_result.h"
#include "health_state.h"
#include "result_to_endogenous_adapter.h"
#include "single_vector_dp_storage.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace healthcaredp {
template <typename Result>
class HealthDPStorage
    : public genericdp::SingleVectorDPStorage<healthcare::HealthState, Result> {
public:
  HealthDPStorage(int max_periods, int max_remaining_cash);
  bool IsTerminalState(const healthcare::HealthState &state) const override;

protected:
  virtual bool
  IsValidState(const healthcare::HealthState &state) const override;
  virtual int GetIndex(const healthcare::HealthState &state) const override;

private:
  int max_periods_;
  int max_remaining_cash_;
  const int sub_table_size_;
  const int row_size_;
};

template <typename Result>
HealthDPStorage<Result>::HealthDPStorage(int max_periods, int max_remaining_cash)
    : max_periods_(max_periods), max_remaining_cash_(max_remaining_cash),
      sub_table_size_((100 + 1) * (max_remaining_cash + 1)),
      row_size_(max_remaining_cash_ + 1) {
  this->result_table_.resize(max_periods * sub_table_size_);
  this->value_table_.resize(max_periods_ * sub_table_size_, 0);
  this->is_stored_table_.resize(max_periods_ * sub_table_size_, false);
}

template <typename Result>
bool HealthDPStorage<Result>::IsTerminalState(
    const healthcare::HealthState &state) const {
  return state.period > max_periods_ || !healthcare::IsAlive(state);
}

template <typename Result>
int HealthDPStorage<Result>::GetIndex(const healthcare::HealthState &state) const {
  if (IsValidState(state)) {
    return ((state.period - 1) * sub_table_size_) + (state.health * row_size_) +
           state.cash;
  } else {
    throw std::out_of_range("HealthState out of range.");
  }
}

template <typename Result>
bool HealthDPStorage<Result>::IsValidState(const healthcare::HealthState &state) const {
  return state.period > 0 && state.period <= max_periods_ &&
         state.health >= 0 && state.health <= 100 && state.cash >= 0 &&
         state.cash <= max_remaining_cash_;
}

} // namespace healthcaredp
#endif // _HEALTH_DP_STORAGE_H_
