#ifndef _HEALTHCAREDP_CONFIGURATION_DEGEN_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_DEGEN_READER_H_

#include <string>

#include "healthcare/degeneration.h"

#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Degeneration> ReadDegeneration(
    boost::property_tree::ptree degen_config, int max_age);
std::unique_ptr<const healthcare::Degeneration> ReadLinearDegen(
    boost::property_tree::ptree degen_config);

}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_DEGEN_READER_H_
