#ifndef _HEALTHCAREDP_CONFIGURATION_JOY_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_JOY_READER_H_

#include <string>

#include "healthcare/enjoyment.h"

#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Enjoyment> ReadEnjoyment(
    boost::property_tree::ptree joy_config, int max_budget);
std::unique_ptr<const healthcare::Enjoyment> ReadFractionalJoy(
    boost::property_tree::ptree joy_config);

}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_JOY_READER_H_
