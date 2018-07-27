#ifndef _HEALTHCAREDP_CONFIGURATION_INSURANCE_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_INSURANCE_READER_H_

#include <string>

#include "healthcare/insurance.h"
#include "healthcaredp/configuration.h"

#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Insurance> ReadInsurance(
    boost::property_tree::ptree insure_config, const Configuration& config);
std::unique_ptr<const healthcare::Insurance> ReadActuarialInsurance(
    boost::property_tree::ptree insure_config, const Configuration& config);
std::unique_ptr<const healthcare::Insurance> ReadFixedCostInsurance(
    boost::property_tree::ptree insure_config);
}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_INSURANCE_READER_H_
