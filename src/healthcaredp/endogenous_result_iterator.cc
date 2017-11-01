#include "endogenous_result_iterator.h"
#include "health_state.h"
#include "investment.h"
#include "period_result.h"
#include "period_result_factory.h"

#include <algorithm>
#include <iostream>

namespace healthcaredp {
EndogenousResultIterator::EndogenousResultIterator(
    const healthcare::PeriodResultFactory &state_factory,
    const healthcare::HealthState &state, int max_remaining_cash)
    : state_factory_(state_factory)
    , invest_state_(state)
    , cur_investment_(0, 0)
    , max_remaining_cash_(max_remaining_cash)
    , cached_result_() {
  cur_investment_.life_investment = std::max(0, invest_state_.cash - max_remaining_cash_);
  ChangeHealthInvestment(0);;
  prev_health_regained_ = cur_health_regained_;
  cached_result_ = std::make_unique<ResultToEndogenousAdapter>(state_factory_.GetPeriodResult(invest_state_, cur_investment_));
  state_ = GetEndogenousResult();
}

EndogenousResultIterator &EndogenousResultIterator::operator++() {
  if (HasRemainingCash()) {
    cur_investment_.life_investment += 1;
    state_ = GetEndogenousResult();
    return *this;
  }

  if (SetNextHealthInvestmentIfAble()) {
    cur_investment_.life_investment =
        std::max(0, invest_state_.cash - cur_investment_.health_investment -
                        max_remaining_cash_);
    state_ = GetEndogenousResult();
    return *this;
  }

  done_ = true;
  return *this;
}

bool EndogenousResultIterator::SetNextHealthInvestmentIfAble() {
  while (cur_investment_.health_investment < invest_state_.cash) {
    ChangeHealthInvestment(cur_investment_.health_investment + 1);
    if (cur_health_regained_ != prev_health_regained_) {
      prev_health_regained_ = cur_health_regained_;
      return true;
    }
  }
  return false;
}

void EndogenousResultIterator::ChangeHealthInvestment(int cur_health_investment) {
  cur_investment_.health_investment = cur_health_investment;
  cur_health_regained_ = GetHealthRegained();
}

int EndogenousResultIterator::GetHealthRegained() const {
  return state_factory_.GetHealthRegained(invest_state_, cur_investment_);
}

double EndogenousResultIterator::GetLifeEnjoyment() const {
  return state_factory_.GetLifeEnjoyment(invest_state_, cur_investment_);
}

bool EndogenousResultIterator::HasRemainingCash() const {
  return cur_investment_.life_investment + cur_investment_.health_investment <
         invest_state_.cash;
}

ResultToEndogenousAdapter *EndogenousResultIterator::GetEndogenousResult() {
  int end_health = invest_state_.health + cur_health_regained_;
  int end_cash = invest_state_.cash - cur_investment_.life_investment -
                 cur_investment_.health_investment;
  double enjoyment = state_factory_.CalculateLifeEnjoyment(end_health, cur_investment_.life_investment);

  cached_result_->SetInvestment(cur_investment_);
  cached_result_->UpdateEndState(end_health, end_cash, enjoyment);
  return cached_result_.get();
}
} // namespace healthcaredp
