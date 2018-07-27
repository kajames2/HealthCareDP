#include "healthcaredp/configuration/insurance_reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcare/insurance.h"
#include "healthcare/insurance/actuarial.h"
#include "healthcare/insurance/fixed_cost.h"
#include "healthcare/insurance/precompute_decorator.h"
#include "healthcaredp/configuration.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Insurance> ReadInsurance(
    boost::property_tree::ptree insure_config, const Configuration& config) {
  std::string type = insure_config.get<std::string>("type");
  std::unique_ptr<const healthcare::Insurance> insure;
  if (type == "Actuarial") {
    insure = ReadActuarialInsurance(insure_config, config);
  } else if (type == "FixedCost") {
    insure = ReadFixedCostInsurance(insure_config);
  } else {
    assert(false && "Unsupported Insurance type");
    insure = std::unique_ptr<const healthcare::Insurance>();
  }
  insure = std::make_unique<const healthcare::insurance::PrecomputeDecorator>(
      *insure, config.max_age, config.n_fitnesses);
  return insure;
}

std::unique_ptr<const healthcare::Insurance> ReadActuarialInsurance(
    boost::property_tree::ptree insure_config, const Configuration& config) {
  float scale = insure_config.get<float>("scale");
  return std::make_unique<healthcare::insurance::Actuarial>(
      scale, config.regen, config.degen, config.shock_size, config.shock_prob);
}

std::unique_ptr<const healthcare::Insurance> ReadFixedCostInsurance(
    boost::property_tree::ptree insure_config) {
  int premium = insure_config.get<float>("premium");
  return std::make_unique<healthcare::insurance::FixedCost>(premium);
}

}  // namespace configuration
}  // namespace healthcaredp
