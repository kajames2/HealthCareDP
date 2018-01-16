#ifndef _DP_HEALTH_STATE_H_
#define _DP_HEALTH_STATE_H_

#include "dp_state.h"
#include "health_state.h"

#include <memory>

namespace healthcaredp {
class DPHealthState : public genericdp::DPState,
                      public healthcare::HealthState {
public:
  DPHealthState()
      : harvest(0), investment_health(0), investment_cash(0),
        life_investment(0), health_investment(0), end_health(0), end_cash(0),
        life_enjoyment(0), shock(false), HealthState(), DPState() {}
  DPHealthState(const healthcare::HealthState &state)
      : healthcare::HealthState(state), harvest(0), investment_health(0), investment_cash(0),
        life_investment(0), health_investment(0), end_health(0), end_cash(0),
        life_enjoyment(0), shock(false), DPState() {}
  int harvest;
  int investment_health;
  int investment_cash;

  int life_investment;
  int health_investment;

  int end_health;
  int end_cash;
  double life_enjoyment;

  bool shock;
};
} // namespace genericdp
#endif // _DP_HEALTH_STATE_H_
