#ifndef _HEALTHCAREDP_HEALTH_DP_RETIREMENT_STORAGE_H_
#define _HEALTHCAREDP_HEALTH_DP_RETIREMENT_STORAGE_H_

#include "genericdp/dp_storage_single_vector.h"
#include "healthcare/health_state.h"

namespace healthcaredp {
template <class T>
class HealthDPRetirementStorage : public genericdp::DPStorageSingleVector<T> {
 public:
  HealthDPRetirementStorage(int max_periods, int max_remaining_cash,
                            int max_working_harvest);
  bool IsTerminalState(const T &state) const override;

 protected:
  virtual bool IsValidState(const T &state) const override;
  virtual int GetIndex(const T &state) const override;

 private:
  int max_periods_;
  int max_remaining_cash_;
  int max_working_harvest_;
  const int period_table_size_;
  const int period_health_table_size_;
  const int period_health_cash_row_size_;
};

template <class T>
HealthDPRetirementStorage<T>::HealthDPRetirementStorage(int max_periods,
                                                        int max_remaining_cash,
                                                        int max_working_harvest)
    : max_periods_(max_periods),
      max_remaining_cash_(max_remaining_cash),
      max_working_harvest_(max_working_harvest),
      period_table_size_((100 + 1) * (max_remaining_cash + 1) *
                         (max_working_harvest + 1)),
      period_health_table_size_((max_remaining_cash + 1) *
                                (max_working_harvest + 1)),
      period_health_cash_row_size_(max_working_harvest + 1),
      genericdp::DPStorageSingleVector<T>(max_periods * (100 + 1) *
                                          (max_remaining_cash + 1) *
                                          (max_working_harvest + 1)) {}

template <class T>
bool HealthDPRetirementStorage<T>::IsTerminalState(const T &state) const {
  return state.period > max_periods_ || !healthcare::IsAlive(state);
}

template <class T>
int HealthDPRetirementStorage<T>::GetIndex(const T &state) const {
  return ((state.period - 1) * period_table_size_) +
         (state.health * period_health_table_size_) +
         (state.cash * period_health_cash_row_size_) +
         state.total_working_harvest;
}

template <class T>
bool HealthDPRetirementStorage<T>::IsValidState(const T &state) const {
  return state.period > 0 && state.period <= max_periods_ &&
         state.health >= 0 && state.health <= 100 && state.cash >= 0 &&
         state.cash <= max_remaining_cash_ &&
         state.total_working_harvest <= max_working_harvest_;
}
}  // namespace healthcaredp
#endif  // _HEALTHCAREDP_HEALTH_DP_RETIREMENT_STORAGE_H_
