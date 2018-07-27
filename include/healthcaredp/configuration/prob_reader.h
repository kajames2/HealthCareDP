#ifndef _HEALTHCAREDP_CONFIGURATION_PROB_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_PROB_READER_H_

#include <string>

#include "healthcare/probability.h"

#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Probability> ReadProb(
    boost::property_tree::ptree prob_config, int max_age);
std::unique_ptr<const healthcare::Probability> ReadHalfHalfProb(
    boost::property_tree::ptree prob_config, int max_age);
}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_PROB_READER_H_
