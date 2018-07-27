#include "healthcaredp/configuration/joy_reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcare/enjoyment.h"
#include "healthcare/enjoyment/precompute_decorator.h"
#include "healthcare/enjoyment/fractional.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Enjoyment> ReadEnjoyment(
    boost::property_tree::ptree joy_config, int max_budget) {
  std::string type = joy_config.get<std::string>("type");
  std::unique_ptr<const healthcare::Enjoyment> joy;
  if (type == "Fractional") {
    joy = ReadFractionalJoy(joy_config);
  } else {
    assert(false && "Unsupported Enjoyment type");
    joy = std::unique_ptr<const healthcare::Enjoyment>();
  }
  joy = std::make_unique<const healthcare::enjoyment::PrecomputeDecorator>(
      *joy, max_budget);
  return joy;
}

std::unique_ptr<const healthcare::Enjoyment> ReadFractionalJoy(
    boost::property_tree::ptree joy_config) {
  float j = joy_config.get<float>("j");
  return std::make_unique<healthcare::enjoyment::Fractional>(j);
}

}  // namespace configuration
}  // namespace healthcaredp
