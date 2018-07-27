#ifndef _HEALTHCAREDP_CONFIGURATION_JOB_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_JOB_READER_H_

#include <string>

#include "healthcare/job.h"

#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Job> ReadJob(
    boost::property_tree::ptree job_config);
std::unique_ptr<const healthcare::Job> ReadLinearJob(
    boost::property_tree::ptree job_config);
std::unique_ptr<const healthcare::Job> ReadFlatJob(
    boost::property_tree::ptree job_config);

}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_JOB_READER_H_
