#ifndef _HEALTHCAREDP_CONFIGURATION_SHOCK_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_SHOCK_READER_H_

#include <string>

#include "healthcare/shock.h"

#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Shock> ReadShock(
    boost::property_tree::ptree shock_config, int n_fitnesses);
std::unique_ptr<const healthcare::Shock> ReadExponentialDecayShock(
    boost::property_tree::ptree shock_config);
}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_SHOCK_READER_H_
