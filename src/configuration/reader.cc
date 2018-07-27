#include "healthcaredp/configuration/reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcaredp/configuration.h"
#include "healthcaredp/configuration/degen_reader.h"
#include "healthcaredp/configuration/job_reader.h"
#include "healthcaredp/configuration/regen_reader.h"
#include "healthcaredp/configuration/joy_reader.h"
#include "healthcaredp/configuration/fitness_reader.h"
#include "healthcaredp/configuration/prob_reader.h"
#include "healthcaredp/configuration/shock_reader.h"
#include "healthcaredp/configuration/insurance_reader.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace boost::property_tree;

namespace healthcaredp {
namespace configuration {

int CalculateMaxBudget(const Configuration& config);

Configuration ReadConfigurationFile(std::string filename) {
  ptree root;
  read_json(filename, root);
  Configuration config;

  assert(root.count("max_age"));
  assert(root.count("max_savings"));
  assert(root.count("job"));
  assert(root.count("regeneration"));
  config.max_age = root.get<int>("max_age");
  config.min_savings = root.get<int>("min_savings", 0);
  config.max_savings = root.get<int>("max_savings");
  config.discount = root.get<float>("discount", 1.0);
  config.job = std::move(ReadJob(root.get_child("job")));
  config.max_budget = CalculateMaxBudget(config);
  config.regen = ReadRegeneration(root.get_child("regeneration"), config.max_budget);
  config.degen =
      ReadDegeneration(root.get_child("degeneration"), config.max_age);
  config.joy =
      ReadEnjoyment(root.get_child("enjoyment"), config.max_budget);
  config.fitness = ReadFitness(root.get_child("fitness"), config.max_budget);
  config.n_fitnesses = ReadNFitnesses(root.get_child("fitness"));
  config.shock_prob = ReadProb(root.get_child("probability"), config.max_age);
  config.shock_size = ReadShock(root.get_child("shock"), config.n_fitnesses);
  config.insure = ReadInsurance(root.get_child("insurance"), config);
  return config;
}

int CalculateMaxBudget(const Configuration& config) {
  int max_income = -1;
  for (int age = 1; age <= config.max_age; ++age) {
    for (int health = 1; health <= 100; ++health) {
      if (config.job->GetEarnings(age, health) > max_income) {
        max_income = config.job->GetEarnings(age, health);
      }
    }
  }
  return max_income + config.max_savings;
}

}  // namespace configuration
}  // namespace healthcaredp
