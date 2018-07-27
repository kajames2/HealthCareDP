#ifndef _HEALTHCAREDP_CONFIGURATION_H_
#define _HEALTHCAREDP_CONFIGURATION_H_

#include <memory>
#include "healthcare/job.h"
#include "healthcare/regeneration.h"
#include "healthcare/degeneration.h"
#include "healthcare/enjoyment.h"
#include "healthcare/probability.h"
#include "healthcare/shock.h"
#include "healthcare/fitness.h"
#include "healthcare/insurance.h"

namespace healthcaredp {

struct Configuration {
  int max_age;
  int max_savings;
  int min_savings;
  int max_budget;
  int n_fitnesses;
  float discount;
  std::shared_ptr<const healthcare::Job> job;
  std::shared_ptr<const healthcare::Regeneration> regen;
  std::shared_ptr<const healthcare::Degeneration> degen;
  std::shared_ptr<const healthcare::Enjoyment> joy;
  std::shared_ptr<const healthcare::Probability> shock_prob;
  std::shared_ptr<const healthcare::Shock> shock_size;
  std::shared_ptr<const healthcare::Fitness> fitness;
  std::shared_ptr<const healthcare::Insurance> insure;
};

}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_H_
