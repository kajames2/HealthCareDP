#ifndef _HEALTH_DP_RETIREMENT_STORAGE_H_
#define _HEALTH_DP_RETIREMENT_STORAGE_H_

#include "health_state.h"
#include "single_vector_dp_storage.h"

#include <memory>
#include <string>
#include <vector>

namespace healthcaredp {
template <typename Result>
class HealthDPRetirementStorage
    : public genericdp::SingleVectorDPStorage<healthcare::HealthState, Result> {
public:
  HealthDPRetirementStorage(int max_periods, int max_remaining_cash,
                            int max_working_harvest);
  bool IsTerminalState(const healthcare::HealthState &state) const override;

protected:
  virtual bool
  IsValidState(const healthcare::HealthState &state) const override;
  virtual int GetIndex(const healthcare::HealthState &state) const override;

private:
  int max_periods_;
  int max_remaining_cash_;
  int max_working_harvest_;
  const int period_table_size_;
  const int period_health_table_size_;
  const int period_health_cash_row_size_;
};

template <typename Result>
HealthDPRetirementStorage<Result>::HealthDPRetirementStorage(int max_periods,
                                                     int max_remaining_cash,
                                                     int max_working_harvest)
    : max_periods_(max_periods), max_remaining_cash_(max_remaining_cash),
      max_working_harvest_(max_working_harvest),
      period_table_size_((100 + 1) * (max_remaining_cash + 1) *
                         (max_working_harvest + 1)),
      period_health_table_size_((max_remaining_cash + 1) *
                                (max_working_harvest + 1)),
      period_health_cash_row_size_(max_working_harvest_ + 1) {
  this->result_table_.resize(max_periods * period_table_size_);
  this->value_table_.resize(max_periods_ * period_table_size_, 0);
  this->is_stored_table_.resize(max_periods_ * period_table_size_, false);
}

template <typename Result>
bool HealthDPRetirementStorage<Result>::IsTerminalState(
    const healthcare::HealthState &state) const {
  return state.period > max_periods_ || !healthcare::IsAlive(state);
}

template <typename Result>
int HealthDPRetirementStorage<Result>::GetIndex(
    const healthcare::HealthState &state) const {
  if (IsValidState(state)) {
    return ((state.period - 1) * period_table_size_) +
           (state.health * period_health_table_size_) +
           (state.cash * period_health_cash_row_size_) +
           state.total_working_harvest;
  } else {
    throw std::out_of_range("HealthState out of range.");
  }
}

template <typename Result>
bool HealthDPRetirementStorage<Result>::IsValidState(
    const healthcare::HealthState &state) const {
  return state.period > 0 && state.period <= max_periods_ &&
         state.health >= 0 && state.health <= 100 && state.cash >= 0 &&
         state.cash <= max_remaining_cash_ &&
         state.total_working_harvest <= max_working_harvest_;
}
} // namespace healthcaredp
#endif // _HEALTH_DP_RETIREMENT_STORAGE_H_
