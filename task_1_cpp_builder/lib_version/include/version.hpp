#pragma once

#include <string>

namespace version {

/**
 * @struct Struct representing a version number of the project.
 */
struct Version {
    int major;
    int minor;
    int build;

    std::string ToString() const;
};

/**
 * @brief Returns a version of the current build.
 */
Version get_project_version();

}