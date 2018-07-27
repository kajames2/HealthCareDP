#include "healthcaredp/configuration/regen_reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcare/regeneration.h"
#include "healthcare/regeneration/precompute_decorator.h"
#include "healthcare/regeneration/shifted_logistic_rounded.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Regeneration> ReadRegeneration(
    boost::property_tree::ptree regen_config, int max_budget) {
  std::string type = regen_config.get<std::string>("type");
  std::unique_ptr<const healthcare::Regeneration> regen;
  if (type == "ShiftedLogisticRounded") {
    regen = ReadShiftedLogisticRoundedRegen(regen_config);
  } else {
    assert(false && "Unsupported Regen type");
    regen = std::unique_ptr<const healthcare::Regeneration>();
  }
  regen = std::make_unique<const healthcare::regeneration::PrecomputeDecorator>(
      *regen, max_budget);
  return regen;
}

std::unique_ptr<const healthcare::Regeneration> ReadShiftedLogisticRoundedRegen(
    boost::property_tree::ptree regen_config) {
  float k = regen_config.get<float>("k");
  return std::make_unique<healthcare::regeneration::ShiftedLogisticRounded>(k);
}

}  // namespace configuration
}  // namespace healthcaredp
