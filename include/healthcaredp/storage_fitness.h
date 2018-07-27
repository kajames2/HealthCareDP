#ifndef _HEALTHCAREDP_STORAGE_FITNESS_H_
#define _HEALTHCAREDP_STORAGE_FITNESS_H_

#include <iostream>

#include "genericdp/result_set.h"
#include "genericdp/storage/single_vector.h"

namespace healthcaredp {
template <typename InState, typename Policy>
class StorageFitness : public genericdp::storage::SingleVector<
                           InState, genericdp::ResultSet<InState, Policy>> {
 public:
  StorageFitness(int max_age, int max_payout, int max_cash, int min_cash);
  inline bool IsTerminalState(const InState& state) const override;

  friend std::ostream& operator<<(std::ostream& out, const StorageFitness& s) {
    out << "Age,Health,Cash,PayoutCode,NextAge,NextHealth,NextCash,"
           "NextPayoutCode,HealthSpending,JoySpending,FitnessSpending,"
           "InsureSpending,Probability,Enjoyment,FutureValue,Value\n";
    for (int age = 1; age < s.max_age_; ++age) {
      for (int payout = 0; payout <= s.max_payout_; ++payout) {
        for (int health = 1; health <= 100; ++health) {
          for (int cash = s.min_cash_; cash <= s.max_cash_; ++cash) {
            InState state = InState{age, health, cash, payout};
            auto opt = s.GetOptimalResult(state);
            for (int i = 0; i < opt.size(); ++i) {
              out << state << "," << opt[i] << '\n';
            }
          }
        }
        out << std::flush;
      }
    }
    return out;
  }

 private:
  inline bool IsValidState(const InState& state) const override;
  inline int GetIndex(const InState& state) const override;

  const int max_age_;
  const int max_cash_;
  const int min_cash_;
  const int max_payout_;
  const int sub_sub_table_size_;
  const int sub_table_size_;
  const int row_size_;
};

template <typename InState, typename Policy>
StorageFitness<InState, Policy>::StorageFitness(int max_age, int max_payout,
                                                int max_cash, int min_cash)
    : max_age_(max_age),
      max_cash_(max_cash),
      min_cash_(min_cash),
      max_payout_(max_payout),
      sub_table_size_((max_payout + 1) * 101 * (max_cash - min_cash + 1)),
      sub_sub_table_size_(101 * (max_cash - min_cash + 1)),
      row_size_(max_cash_ - min_cash + 1),
      genericdp::storage::SingleVector<InState,
                                       genericdp::ResultSet<InState, Policy>>(
          max_age * (max_payout + 1) * 101 * (max_cash - min_cash + 1)) {}

template <typename InState, typename Policy>
bool StorageFitness<InState, Policy>::IsTerminalState(
    const InState& state) const {
  return state.age > max_age_ || state.health <= 0;
}

template <typename InState, typename Policy>
int StorageFitness<InState, Policy>::GetIndex(const InState& state) const {
  if (!IsValidState(state)) {
    std::cerr << state << std::endl;
    assert(false);
  }
  return ((state.age - 1) * sub_table_size_) +
         (state.payout_code * sub_sub_table_size_) +
         (state.health * row_size_) + (state.cash - min_cash_);
}

template <typename InState, typename Policy>
bool StorageFitness<InState, Policy>::IsValidState(const InState& state) const {
  return state.age > 0 && state.age <= max_age_ && state.payout_code >= 0 &&
         state.payout_code <= max_payout_ && state.health >= 0 &&
         state.health <= 100 && state.cash >= min_cash_ &&
         state.cash <= max_cash_;
}

}  // namespace healthcaredp
#endif  // _HEALTHCAREDP_STORAGE_FITNESS_H_
