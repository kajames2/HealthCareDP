#include "healthcaredp/configuration/prob_reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcare/probability.h"
#include "healthcare/probability/age_defined.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Probability> ReadProb(
    boost::property_tree::ptree prob_config, int max_age) {
  std::string type = prob_config.get<std::string>("type");
  std::unique_ptr<const healthcare::Probability> prob;
  if (type == "HalfHalf") {
    prob = ReadHalfHalfProb(prob_config, max_age);
  } else {
    assert(false && "Unsupported Probability type");
    prob = std::unique_ptr<const healthcare::Probability>();
  }
  return prob;
}

std::unique_ptr<const healthcare::Probability> ReadHalfHalfProb(
    boost::property_tree::ptree prob_config, int max_age) {
  float p1 = prob_config.get<float>("p1");
  float p2 = prob_config.get<float>("p2");
  return std::make_unique<healthcare::probability::AgeDefined>(healthcare::probability::AgeDefined::TwoTiered(p1, max_age/2, p2, max_age));
}

}  // namespace configuration
}  // namespace healthcaredp
