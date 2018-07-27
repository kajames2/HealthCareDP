#ifndef _HEALTHCAREDP_FASTDP_DECISION_CACHE_FITNESS_H_
#define _HEALTHCAREDP_FASTDP_DECISION_CACHE_FITNESS_H_

#include <algorithm>
#include <array>
#include <numeric>
#include <vector>

#include <iostream>

#include "healthcare/decision_fitness.h"
#include "healthcaredp/configuration.h"

namespace healthcaredp {
namespace fastdp {

class DecisionCacheFitness {
 public:
  DecisionCacheFitness(int age, Configuration config);

  typename std::vector<healthcare::DecisionFitness>::const_iterator begin(
      int health, int budget) const {
    if (budget > config_.max_savings) {
      return decisions_[health].begin() +
             counts_[health][budget - config_.max_savings - 1];
    }
    return decisions_[health].begin();
  }

  typename std::vector<healthcare::DecisionFitness>::const_iterator end(
      int health, int budget) const {
    return decisions_[health].begin() + counts_[health][budget];
  }
  int GetDecisionCount(int health, int budget) const {
    if (budget > config_.max_savings) {
      return counts_[health][budget] -
             counts_[health][budget - config_.max_savings - 1];
    }
    return counts_[health][budget];
  }

 private:
  const Configuration config_;
  const std::vector<std::vector<int>> counts_;
  const std::vector<std::vector<healthcare::DecisionFitness>> decisions_;

  std::vector<std::vector<int>> BuildCounts(int age) const;
  std::vector<int> BuildHealthCounts(int age, int health) const;
  int NOptionsExactBudget(int age, int health, int budget) const;
  int NOptionsExactBudgetRegain(int age, int health, int budget) const;
  int NOptionsExactBudgetRegainFit(int age, int health, int budget,
                                   int fitness) const;
  std::vector<std::vector<healthcare::DecisionFitness>> BuildDecisions(
      int age) const;
  std::vector<healthcare::DecisionFitness> BuildHealthDecisions(
      int age, int health) const;
};

}  // namespace fastdp
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_FASTDP_DECISION_CACHE_FITNESS_H_
