#include <sstream>

#include <version.hpp>
#include <lib_version/src/version_consts.hpp>

namespace version {

std::string Version::ToString() const {
    std::stringstream ss;
    ss << PROJECT_VERSION_MAJOR << "."
       << PROJECT_VERSION_MINOR << "."
       << PROJECT_VERSION_PATCH;
    return ss.str();
}

Version get_project_version() {
    return Version{
      PROJECT_VERSION_MAJOR,  // major
      PROJECT_VERSION_MINOR,  // minor
      PROJECT_VERSION_PATCH,  // build
    };
}

}