#ifndef _HEALTHCAREDP_CONFIGURATION_READER_H_
#define _HEALTHCAREDP_CONFIGURATION_READER_H_

#include <string>

#include "healthcaredp/configuration.h"

namespace healthcaredp {
namespace configuration {

Configuration ReadConfigurationFile(std::string filename);

}  // namespace configuration
}  // namespace healthcaredp

#endif  // _HEALTHCAREDP_CONFIGURATION_READER_H_
