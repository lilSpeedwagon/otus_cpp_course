#include <iostream>

#include <version.hpp>

int main() {
    const auto version_str = version::get_project_version().ToString();
    std::cout << "Current build version: " << version_str << "\n"
              << "Hello, world!\n";
    return 0;
}