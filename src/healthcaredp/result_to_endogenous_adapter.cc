#include "result_to_endogenous_adapter.h"

#include <memory>

namespace healthcaredp {
ResultToEndogenousAdapter::ResultToEndogenousAdapter(
    healthcare::PeriodResult end_state)
    : end_state_(end_state) {}

healthcare::HealthState ResultToEndogenousAdapter::GetState() const {
  return end_state_.state;
}

double ResultToEndogenousAdapter::GetValue() const {
  return end_state_.enjoyment;
}

std::unique_ptr<genericdp::EndogenousState<healthcare::HealthState>>
ResultToEndogenousAdapter::Clone() const {
  return std::make_unique<ResultToEndogenousAdapter>(end_state_);
}

std::string ResultToEndogenousAdapter::GetHeader() const {
  return std::string("Health Expenditure") + ", " + "Life Expenditure" + ", " +
         "End Health" + ", " + "End Cash" + ", " + "Life Enjoyment";
}

std::string ResultToEndogenousAdapter::GetString() const {
  healthcare::HealthState hstate = end_state_.state;
  return std::to_string(end_state_.investment.health_investment) + ", " +
         std::to_string(end_state_.investment.life_investment) + ", " +
         std::to_string(hstate.health) + ", " + std::to_string(hstate.cash) +
         ", " + std::to_string(end_state_.enjoyment);
}

void ResultToEndogenousAdapter::SetInvestment(
    healthcare::Investment investment) {
  end_state_.investment = investment;
}

void ResultToEndogenousAdapter::UpdateEndState(int end_health, int end_cash,
                                               double life_enjoyment) {
  end_state_.enjoyment = life_enjoyment;
  end_state_.state.health = end_health;
  end_state_.state.cash = end_cash;
}
}
