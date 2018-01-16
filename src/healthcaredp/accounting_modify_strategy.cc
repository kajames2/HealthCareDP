#include "accounting_modify_strategy.h"

namespace healthcaredp {

AccountingModifyStrategy::AccountingModifyStrategy() {}

void AccountingModifyStrategy::Modify(DPHealthState *state) const {
  state->end_health = state->health;
  state->end_cash = state->cash;
  state->immediate_value = state->life_enjoyment;
  state->period++;
}
}
