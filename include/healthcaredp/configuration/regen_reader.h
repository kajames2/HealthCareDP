#ifndef _HEALTHCAREDP_CONFIGURATION_REGEN_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_REGEN_READER_H_

#include <string>

#include "healthcare/regeneration.h"

#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Regeneration> ReadRegeneration(
    boost::property_tree::ptree regen_config, int max_budget);
std::unique_ptr<const healthcare::Regeneration> ReadShiftedLogisticRoundedRegen(
    boost::property_tree::ptree regen_config);

}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_REGEN_READER_H_
