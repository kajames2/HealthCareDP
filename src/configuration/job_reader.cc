#include "healthcaredp/configuration/job_reader.h"

#include <cassert>
#include <memory>
#include <string>

#include "healthcare/job.h"
#include "healthcare/job/flat.h"
#include "healthcare/job/linear.h"
#include "healthcare/job/min_decorator.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace healthcaredp {
namespace configuration {

std::unique_ptr<const healthcare::Job> ReadJob(
    boost::property_tree::ptree job_config) {
  std::string type = job_config.get<std::string>("type");
  if (type == "Linear") {
    return ReadLinearJob(job_config);
  } else if (type == "Flat") {
    return ReadFlatJob(job_config);
  } else {
    assert(false && "Unsupported job type");
    return std::unique_ptr<const healthcare::Job>();
  }
}

std::unique_ptr<const healthcare::Job> ReadLinearJob(
    boost::property_tree::ptree job_config) {
  int intercept = job_config.get<int>("intercept", 0);
  float slope = job_config.get<float>("slope");
  std::unique_ptr<const healthcare::Job> job =
      std::make_unique<healthcare::job::Linear>(slope, intercept);
  if (job_config.count("min")) {
    int min = job_config.get<int>("min");
    job = std::make_unique<healthcare::job::MinDecorator>(min, std::move(job));
  }
  return job;
}

std::unique_ptr<const healthcare::Job> ReadFlatJob(
    boost::property_tree::ptree job_config) {
  int rate = job_config.get<int>("rate");
  return std::make_unique<healthcare::job::Flat>(rate);
}

}  // namespace configuration
}  // namespace healthcaredp
