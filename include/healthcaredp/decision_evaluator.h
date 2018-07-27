#ifndef _HEALTHCAREDP_DECISION_EVALUATOR_H_
#define _HEALTHCAREDP_DECISION_EVALUATOR_H_

#include "healthcaredp/configuration.h"
#include "genericdp/result.h"
#include "healthcare/person_fitness.h"
#include "healthcare/decision_fitness.h"

namespace helthcaredp {

struct DecisionPair {
  genericdp::Result<healthcare::PersonFitness, healthcare::DecisionFitness> no_shock;
  genericdp::Result<healthcare::PersonFitness, healthcare::DecisionFitness> shock;
};

class DecisionEvaluator {
 public:
  using DPPerson = healthcare::PersonFitness;
  using DPPolicy = healthcare::DecisionFitness;
  using PersonDecision = genericdp::Result<DPPerson, DPPolicy>;
  
  DecisionPair DecisionResults(DPPerson state, DPPolicy dec) const;

 private:
  PersonDecision PreShockDecision(DPPerson state, DPPolicy dec) const;
  PersonDecision ApplyDecision(PersonDecision dec) const;
  PersonDecision ApplyDecisionShock(PersonDecision dec) const;
  PersonDecision PostShockDecision(PersonDecision dec) const;
  DPPerson EffectivePayoutState(DPPerson state) const;

  const Configuration config_;
};

}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_DECISION_EVALUATOR_H_
