#ifndef _HEALTHCAREDP_CONFIGURATION_FITNESS_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_FITNESS_READER_H_

#include <string>

#include "healthcare/fitness.h"

#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Fitness> ReadFitness(
    boost::property_tree::ptree fitness_config, int max_budget);
std::unique_ptr<const healthcare::Fitness> ReadDefinedFitness(
    boost::property_tree::ptree fitness_config);
int ReadNFitnesses(boost::property_tree::ptree fitness_config);
int ReadDefinedNFitnesses(boost::property_tree::ptree fitness_config);

}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_FITNESS_READER_H_
