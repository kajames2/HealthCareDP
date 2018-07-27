#ifndef _HEALTHCAREDP_FASTDP_DECISION_CACHE_H_
#define _HEALTHCAREDP_FASTDP_DECISION_CACHE_H_

#include <vector>

#include "healthcare/decision.h"
#include "healthcare/person.h"
#include "healthcare/regeneration.h"

namespace healthcaredp {
namespace fastdp {

class DecisionCache {
 public:
  DecisionCache(int max_savings, int max_budget,
                const healthcare::Regeneration& regen);

  typename std::vector<healthcare::Decision>::const_iterator begin(
      int health, int budget) const {
    if (budget > max_savings_) {
      return decisions_[health].begin() +
             counts_[health][budget - max_savings_ - 1];
    }
    return decisions_[health].begin();
  }

  typename std::vector<healthcare::Decision>::const_iterator end(
      int health, int budget) const {
    return decisions_[health].begin() + counts_[health][budget];
  }

  int GetDecisionCount(int health, int budget) const {
    if (budget > max_savings_) {
      return counts_[health][budget] -
             counts_[health][budget - max_savings_ - 1];
    }
    return counts_[health][budget];
  }

 private:
  const std::vector<std::vector<int>> counts_;
  const std::vector<std::vector<healthcare::Decision>> decisions_;
  int max_savings_;
};

}  // namespace fastdp
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_FASTDP_DECISION_CACHE_H_
