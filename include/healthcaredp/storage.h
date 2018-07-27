#ifndef _HEALTHCAREDP_STORAGE_H_
#define _HEALTHCAREDP_STORAGE_H_

#include <ostream>

#include "genericdp/result.h"
#include "genericdp/storage/single_vector.h"

namespace healthcaredp {
template <typename InState, typename Policy>
class Storage : public genericdp::storage::SingleVector<
                    InState, genericdp::Result<InState, Policy>> {
 public:
  Storage(int max_age, int max_cash, int min_cash = 0);
  inline bool IsTerminalState(const InState& state) const override;

  friend std::ostream& operator<<(std::ostream& out, const Storage& s) {
    out << "Age,Health,Cash,NextAge,NextHealth,NextCash,HealthSpending,"
           "JoySpending,Probability,Joy,FutureValue,Value\n";
    for (int age = 1; age < s.max_age_; ++age) {
      for (int health = 1; health <= 100; ++health) {
        for (int cash = s.min_cash_; cash <= s.max_cash_; ++cash) {
          InState state = InState{age, health, cash};
          auto opt = s.GetOptimalResult(state);
          out << state << "," << opt << '\n';
        }
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
  const int sub_table_size_;
  const int row_size_;
};

template <typename InState, typename Data>
Storage<InState, Data>::Storage(int max_age, int max_cash, int min_cash)
    : max_age_(max_age),
      max_cash_(max_cash),
      min_cash_(min_cash),
      sub_table_size_(101 * (max_cash - min_cash + 1)),
      row_size_(max_cash - min_cash + 1),
      genericdp::storage::SingleVector<InState,
                                       genericdp::Result<InState, Data>>(
          max_age * 101 * (max_cash - min_cash + 1)) {}

template <typename InState, typename Data>
bool Storage<InState, Data>::IsTerminalState(const InState& state) const {
  return state.age > max_age_ || state.health <= 0;
}

template <typename InState, typename Data>
int Storage<InState, Data>::GetIndex(const InState& state) const {
  return ((state.age - 1) * sub_table_size_) + (state.health * row_size_) +
         (state.cash - min_cash_);
}

template <typename InState, typename Data>
bool Storage<InState, Data>::IsValidState(const InState& state) const {
  return state.age > 0 && state.age <= max_age_ && state.health >= 0 &&
         state.health <= 100 && state.cash >= min_cash_ &&
         state.cash <= max_cash_;
}

}  // namespace healthcaredp
#endif  // _HEALTHCAREDP_STORAGE_H_
