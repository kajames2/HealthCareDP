#include "healthcaredp/configuration/degen_reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcare/degeneration.h"
#include "healthcare/degeneration/linear.h"
#include "healthcare/degeneration/precompute_decorator.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Degeneration> ReadDegeneration(
    boost::property_tree::ptree degen_config, int max_age) {
  std::string type = degen_config.get<std::string>("type");
  std::unique_ptr<const healthcare::Degeneration> degen;
  if (type == "Linear") {
    degen = ReadLinearDegen(degen_config);
  } else {
    assert(false && "Unsupported Degen type");
    degen = std::unique_ptr<const healthcare::Degeneration>();
  }
  degen = std::make_unique<const healthcare::degeneration::PrecomputeDecorator>(
      *degen, max_age);
  return degen;
}

std::unique_ptr<const healthcare::Degeneration> ReadLinearDegen(
    boost::property_tree::ptree degen_config) {
  float intercept = degen_config.get<float>("intercept");
  float slope = degen_config.get<float>("slope");
  return std::make_unique<healthcare::degeneration::Linear>(intercept, slope);
}

}  // namespace configuration
}  // namespace healthcaredp
