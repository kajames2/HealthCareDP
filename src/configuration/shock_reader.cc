#include "healthcaredp/configuration/shock_reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcare/shock.h"
#include "healthcare/shock/exponential_decay.h"
#include "healthcare/shock/precompute_decorator.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Shock> ReadShock(
    boost::property_tree::ptree shock_config, int n_fitnesses) {
  std::string type = shock_config.get<std::string>("type");
  std::unique_ptr<const healthcare::Shock> shock;
  if (type == "ExponentialDecay") {
    shock = ReadExponentialDecayShock(shock_config);
  } else {
    assert(false && "Unsupported Shock type");
    shock = std::unique_ptr<const healthcare::Shock>();
  }
  shock = std::make_unique<const healthcare::shock::PrecomputeDecorator>(
      *shock, n_fitnesses);
  return shock;
}

std::unique_ptr<const healthcare::Shock> ReadExponentialDecayShock(
    boost::property_tree::ptree shock_config) {
  float max = shock_config.get<float>("max");
  float rate = shock_config.get<float>("rate");
  return std::make_unique<healthcare::shock::ExponentialDecay>(max, rate);
}

}  // namespace configuration
}  // namespace healthcaredp
