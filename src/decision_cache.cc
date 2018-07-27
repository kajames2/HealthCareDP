#include "healthcaredp/decision_cache.h"
#include "healthcare/decision.h"
#include "healthcare/regeneration.h"

#include <algorithm>
#include <numeric>
#include <vector>

namespace healthcaredp {
namespace fastdp {

using namespace healthcare;

std::vector<std::vector<Decision>> BuildDecisions(int max_budget,
                                                  const Regeneration& regen);
std::vector<Decision> BuildDecisionsGivenHealth(int health, int budget,
                                                const Regeneration& regen);
std::vector<std::vector<int>> BuildCounts(int max_budget,
                                          const Regeneration& regen);
std::vector<int> BuildCountsGivenHealth(int health, int max_budget,
                                        const Regeneration& regen);
int NWaysSpendAllBudget(int health, int budget, const Regeneration& regen);

DecisionCache::DecisionCache(int max_savings, int max_budget,
                             const Regeneration& regen)
    : counts_(BuildCounts(max_budget, regen)),
      decisions_(BuildDecisions(max_budget, regen)),
      max_savings_(max_savings) {}

std::vector<std::vector<int>> BuildCounts(int max_budget,
                                          const Regeneration& regen) {
  std::vector<std::vector<int>> counts;
  counts.reserve(101);
  for (int health = 0; health <= 100; ++health) {
    counts.push_back(BuildCountsGivenHealth(health, max_budget, regen));
  }
  return counts;
}

// Let:
//   N_b = # of ways to spend at most b.
//   M_b = # of ways to spend exactly b.
// Then: N_b = Sum_(i=0)^b(M_i) = M_b + Sum_(i=0)^(b-1)(M_i) = M_b + N_(b-1)
// To find all N_b values, find all M_b values, and then partial sum.
std::vector<int> BuildCountsGivenHealth(int health, int max_budget,
                                        const Regeneration& regen) {
  std::vector<int> counts(max_budget + 1);
  if (health <= 0) {
    std::fill(counts.begin(), counts.end(), 1);
    return counts;
  }
  for (int budget = 0; budget <= max_budget; ++budget) {
    counts[budget] = NWaysSpendAllBudget(health, budget, regen);
  }
  std::partial_sum(counts.begin(), counts.end(), counts.begin());
  return counts;
}

// If spending all budget nets h health, then can spend all budget h+1 ways (0
// health regain + all in joy, 1 health regain + rest in joy, ..., h health
// regain + rest in joy)
int NWaysSpendAllBudget(int health, int budget, const Regeneration& regen) {
  int max_regain =
      std::min(100 - health, regen.GetHealthRegained(health, budget));
  return 1 + max_regain;
}

std::vector<std::vector<Decision>> BuildDecisions(int max_budget,
                                                  const Regeneration& regen) {
  std::vector<std::vector<Decision>> decs;
  for (int health = 0; health <= 100; ++health) {
    decs.push_back(BuildDecisionsGivenHealth(health, max_budget, regen));
  }
  return decs;
}

std::vector<Decision> BuildDecisionsGivenHealth(int health, int budget,
                                                const Regeneration& regen) {
  if (health == 0) {
    return std::vector<Decision>(1);
  }
  std::vector<Decision> decs;
  int max_regain =
      std::min(100 - health, regen.GetHealthRegained(health, budget));
  for (int hr = 0; hr <= max_regain; ++hr) {
    int regen_cost = regen.GetRegainCost(health, hr);
    int rem_budget = budget - regen_cost;
    for (int joy_spending = 0; joy_spending <= rem_budget; ++joy_spending) {
      decs.push_back(Decision{regen_cost, joy_spending});
    }
  }
  std::stable_sort(decs.begin(), decs.end(), [](Decision dec, Decision dec2) {
    return TotalSpending(dec) < TotalSpending(dec2);
  });
  return decs;
}

}  // namespace fastdp
}  // namespace healthcaredp
