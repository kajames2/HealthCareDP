#include <omp.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "genericdp/result.h"
#include "genericdp/result_set.h"
#include "healthcare/decision_fitness.h"
#include "healthcare/person_fitness.h"
#include "healthcaredp/configuration.h"
#include "healthcaredp/configuration/reader.h"
#include "healthcaredp/decision_cache_fitness.h"
#include "healthcaredp/storage.h"
#include "healthcaredp/storage_fitness.h"

using namespace healthcare;
using namespace healthcaredp;
using namespace genericdp;

using DPPerson = healthcare::PersonFitness;
using DPPolicy = healthcare::DecisionFitness;
using PersonDecision = genericdp::Result<DPPerson, DPPolicy>;
using DPStorage = healthcaredp::StorageFitness<DPPerson, DPPolicy>;
using DPDecisionCache = healthcaredp::fastdp::DecisionCacheFitness;

struct DecisionPair {
  genericdp::Result<DPPerson, DPPolicy> no_shock;
  genericdp::Result<DPPerson, DPPolicy> shock;
};

std::pair<boost::filesystem::path, boost::filesystem::path> GetPaths(
    int argc, char** argv);
void RunOptimization(const Configuration& config, std::string out_dir,
                     std::string basename);
std::vector<DPPerson> CreateTemplateStates(int min_savings, int max_savings);
void AddEarnings(std::vector<DPPerson>& states, const Configuration& config);
DPPerson EffectivePayoutState(DPPerson state, const Configuration& config);
DecisionPair DecisionResults(DPPerson state, DPPolicy dec,
                             const Configuration& config);
PersonDecision PreShockDecision(DPPerson state, DPPolicy dec,
                                const Configuration& config);
PersonDecision ApplyDecision(PersonDecision dec, const Configuration& config);
PersonDecision ApplyDecisionShock(PersonDecision dec,
                                  const Configuration& config);
PersonDecision PostShockDecision(PersonDecision dec,
                                 const Configuration& config);

void StoreOptimalDecisions(DPStorage& storage, DPStorage& min_storage,
                           const Configuration& config);
void StoreAgeOptimals(DPStorage& storage, DPStorage& min_storage,
                      const DPDecisionCache& dec_cache,
                      const std::vector<DPPerson>& init_states,
                      const Configuration& config);

template <class T>
std::vector<T> JoinVectors(std::vector<T> v1, std::vector<T> v2);

//  Risk aversion ex:  Take sqrt of LE function.
auto util = [](float joy) -> float {
  return std::round(2.923628109436858 * joy);
};

float ExpectedUtility(const DecisionPair& pair) {
  return pair.no_shock.valuation.value * pair.no_shock.valuation.probability +
         pair.shock.valuation.value * pair.shock.valuation.probability;
}

int main(int argc, char** argv) {
  using namespace boost::filesystem;
  try {
    std::pair<path, path> paths = GetPaths(argc, argv);
    path input = paths.first;
    path out_dir = paths.second;
    const Configuration config =
        configuration::ReadConfigurationFile(input.string());
    RunOptimization(config, out_dir.string(), input.stem().string());
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    return 1;
  } catch (const boost::program_options::error& ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
}

std::pair<boost::filesystem::path, boost::filesystem::path> GetPaths(
    int argc, char** argv) {
  using namespace boost::program_options;
  using namespace boost::filesystem;
  options_description desc("Options");
  desc.add_options()("help,h", "Help screen")(
      "i", value<std::string>(), "Input Filename")("o", value<std::string>(),
                                                   "Output Directory");
  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    throw "";
  }

  if (!vm.count("i")) {
    throw "No input configuration file provided.";
  }

  if (!vm.count("o")) {
    throw "No output directory provided.";
  }
  path input(vm["i"].as<std::string>());
  path out_dir(vm["o"].as<std::string>());
  if (!exists(input) || !is_regular_file(input)) {
    throw "Input file does not exist or is not a regular file.";
  }

  if (!exists(out_dir) || !is_directory(out_dir)) {
    throw "Output directory does not exist or is not a valid directory.";
  }

  return std::pair<path, path>(input, out_dir);
}

void RunOptimization(const Configuration& config, std::string out_dir,
                     std::string basename) {
  DPStorage opt_decisions(config.max_age + 1, 0, config.max_savings,
                          config.min_savings);
  DPStorage min_decisions(config.max_age + 1, 0, config.max_savings,
                          config.min_savings);
  StoreOptimalDecisions(opt_decisions, min_decisions, config);
  std::cerr << "Exporting Optimals" << std::endl;

  std::string max_filename = out_dir + '/' + basename + "_max.csv";
  std::ofstream max_stream(max_filename, std::ofstream::out);
  max_stream << opt_decisions << std::endl;
  max_stream.close();

  std::string min_filename = out_dir + '/' + basename + "_min.csv";
  std::ofstream min_stream(min_filename, std::ofstream::out);
  min_stream << min_decisions << std::endl;
  min_stream.close();
  // for (int fit = 0; fit < config.n_fitnesses; ++fit) {
  //   int s = (*config.shock_size)(fit);
  //   for (int h = 1; h <= 100; h += 1) {
  //     std::cout << config.regen->GetRegainCost(h - s, s) << ",";
  //   }
  //   std::cout << std::endl;
  // }
  // for (int h = 1; h <= 100; h += 1) {
  //   int s = config.degen->GetDegeneration(1, h);
  //   std::cout << config.regen->GetRegainCost(h - s, s) << ",";
  // }
  // std::cout << std::endl;
}

std::vector<DPPerson> CreateTemplateStates(int min_savings, int max_savings) {
  std::vector<DPPerson> init_states;
  init_states.reserve((100 + 1) * (max_savings + 1));
  for (int h = 0; h <= 100; ++h) {
    for (int c = min_savings; c <= max_savings; ++c)
      init_states.push_back(DPPerson{0, h, c, 0});
  }
  return init_states;
}

void StoreOptimalDecisions(DPStorage& storage, DPStorage& storage_min,
                           const Configuration& config) {
  std::vector<DPPerson> init_states =
      CreateTemplateStates(config.min_savings, config.max_savings);
  std::vector<DPPerson> states(init_states.size());
  for (int age = config.max_age; age >= 1; --age) {
    std::cerr << age << ", " << std::flush;
    DPDecisionCache dec_cache(age, config);
    std::cerr << "made_cache..." << std::endl;
    for (auto& state : init_states) {
      state.age = age;
      state.payout_code = 0;
    }
    StoreAgeOptimals(storage, storage_min, dec_cache, init_states, config);
  }
}

void StoreAgeOptimals(DPStorage& storage, DPStorage& storage_min,
                      const DPDecisionCache& dec_cache,
                      const std::vector<DPPerson>& init_states,
                      const Configuration& config) {
  auto states = init_states;
  AddEarnings(states, config);
  std::vector<DecisionPair> dec_states(config.max_budget * 100);
  // Performance critical loop.  Be careful when modifying.
#pragma omp parallel for private(dec_states)
  for (int cur = 0; cur < states.size(); ++cur) {
    DPPerson cur_state = states[cur];
    auto start_it = dec_cache.begin(cur_state.health, cur_state.cash);
    auto end_it = dec_cache.end(cur_state.health, cur_state.cash);

    size_t n_decisions = std::distance(start_it, end_it);
    dec_states.resize(n_decisions);
    std::transform(start_it, end_it, dec_states.begin(),
                   [cur_state, config](DPPolicy dec) -> DecisionPair {
                     return DecisionResults(cur_state, dec, config);
                   });

    std::for_each(dec_states.begin(), dec_states.end(),
                  [&storage, config](DecisionPair& pair) -> void {
                    pair.no_shock.valuation.future_value =
                        storage.GetOptimalValue(pair.no_shock.state);
                    pair.shock.valuation.future_value =
                        storage.GetOptimalValue(pair.shock.state);

                    pair.no_shock.valuation.value +=
                        config.discount * pair.no_shock.valuation.future_value;
                    pair.shock.valuation.value +=
                        config.discount * pair.shock.valuation.future_value;
                  });

    auto opts = std::minmax_element(
        dec_states.begin(), dec_states.end(),
        [](const DecisionPair& p1, const DecisionPair& p2) -> bool {
          return ExpectedUtility(p1) < ExpectedUtility(p2);
        });

    ResultSet<DPPerson, DPPolicy> res_max;
    res_max.AddResult(opts.second->no_shock);
    res_max.AddResult(opts.second->shock);
    storage.StoreOptimalResult(init_states[cur], res_max,
                               ExpectedUtility(*opts.second));

    ResultSet<DPPerson, DPPolicy> res_min;
    res_min.AddResult(opts.first->no_shock);
    res_min.AddResult(opts.first->shock);
    storage_min.StoreOptimalResult(init_states[cur], res_min,
                                   ExpectedUtility(*opts.first));
  }
}

void AddEarnings(std::vector<DPPerson>& states, const Configuration& config) {
  for (auto& state : states) {
    state.cash += config.job->GetEarnings(state.age, state.health);
    state.cash = std::max(state.cash, 0);
  }
}

DecisionPair DecisionResults(DPPerson state, DPPolicy dec,
                             const Configuration& config) {
  PersonDecision res = PreShockDecision(state, dec, config);
  return DecisionPair{ApplyDecision(res, config),
                      ApplyDecisionShock(res, config)};
}

PersonDecision PreShockDecision(DPPerson state, DPPolicy dec,
                                const Configuration& config) {
  state.cash -= TotalSpending(dec);
  state.health +=
      config.regen->GetHealthRegained(state.health, dec.health_spending);
  state.health -= config.degen->GetDegeneration(state.age, state.health);
  return PersonDecision{genericdp::Valuation(), state, dec};
}

PersonDecision ApplyDecision(PersonDecision dec, const Configuration& config) {
  dec.valuation.probability *= (1 - (*config.shock_prob)(dec.state.age));
  dec.state.payout_code = 0;
  return PostShockDecision(dec, config);
}

PersonDecision ApplyDecisionShock(PersonDecision dec,
                                  const Configuration& config) {
  dec.valuation.probability *= (*config.shock_prob)(dec.state.age);
  int fitness = config.fitness->GetFitness(dec.policy.fitness_spending);
  dec.state.health -= (*config.shock_size)(fitness);
  dec.state.payout_code = dec.policy.insurance_spending > 0 ? fitness + 1 : 0;
  return PostShockDecision(dec, config);
}

PersonDecision PostShockDecision(PersonDecision dec,
                                 const Configuration& config) {
  dec.state.health = std::max(0, dec.state.health);
  dec.state.cash = std::min(config.max_savings, dec.state.cash);
  dec.valuation.immediate_value =
      util(config.joy->GetEnjoyment(dec.state.health, dec.policy.joy_spending));
  dec.valuation.value = dec.valuation.immediate_value;
  ++dec.state.age;
  if (dec.state.payout_code > 0) {
    dec.state = EffectivePayoutState(dec.state, config);
  }
  return dec;
}

DPPerson EffectivePayoutState(DPPerson state, const Configuration& config) {
  int fitness = state.payout_code - 1;
  int restored_health;
  if (state.health <= 0) {
    restored_health = 0;
  } else {
    restored_health = (*config.shock_size)(fitness) + state.health;
  }
  int lost_earnings = config.job->GetEarnings(state.age, restored_health) -
                      config.job->GetEarnings(state.age, state.health);
  DPPerson effective_state = state;
  effective_state.health = restored_health;
  effective_state.cash -= lost_earnings;
  effective_state.payout_code = 0;
  return effective_state;
}
