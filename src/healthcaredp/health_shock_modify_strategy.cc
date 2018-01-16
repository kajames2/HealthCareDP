#include "health_shock_modify_strategy.h"

namespace healthcaredp {

HealthShockModifyStrategy::HealthShockModifyStrategy(double prob, int shock_size)
    : prob_(prob)
    , shock_size_(shock_size) {}

void HealthShockModifyStrategy::Modify(DPHealthState *state) const {
  state->probability *= prob_;
  state->health -= shock_size_;
  state->shock = true;
}
}
