#include "healthcaredp/configuration/fitness_reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcare/fitness.h"
#include "healthcare/fitness/defined_costs.h"
#include "healthcare/fitness/precompute_decorator.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Fitness> ReadFitness(
    boost::property_tree::ptree fit_config, int max_budget) {
  std::string type = fit_config.get<std::string>("type");
  std::unique_ptr<const healthcare::Fitness> fit;
  if (type == "Defined") {
    fit = ReadDefinedFitness(fit_config);
  } else {
    assert(false && "Unsupported Fitness type");
    fit = std::unique_ptr<const healthcare::Fitness>();
  }
  int n_fitnesses = ReadNFitnesses(fit_config);
  fit = std::make_unique<const healthcare::fitness::PrecomputeDecorator>(*fit, n_fitnesses, max_budget);
  return fit;
}

std::unique_ptr<const healthcare::Fitness> ReadDefinedFitness(
    boost::property_tree::ptree fit_config) {
  std::vector<int> costs;
  for (auto& cell : fit_config.get_child("costs")) {
    costs.emplace_back(cell.second.get_value<int>());
  }
  return std::make_unique<healthcare::fitness::DefinedCosts>(costs);
}

int ReadNFitnesses(boost::property_tree::ptree fit_config) {
  std::string type = fit_config.get<std::string>("type");
  if (type == "Defined") {
    return ReadDefinedNFitnesses(fit_config);
  } else {
    assert(false && "Unsupported Fitness type");
    return -1;
  }
}

int ReadDefinedNFitnesses(boost::property_tree::ptree fit_config) {
  int count = 0;
  for (auto& cell : fit_config.get_child("costs")) {
    ++count;
  }
  return count;
}

}  // namespace configuration
}  // namespace healthcaredp
