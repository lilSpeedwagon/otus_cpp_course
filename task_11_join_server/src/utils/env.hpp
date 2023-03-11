#pragma once

#include <optional>
#include <string>


namespace join_server::utils {

template<typename T, typename std::enable_if<std::is_same_v<T, std::string>, bool>::type = true>
T GetEnv(const std::string& variable_name, T default_value) {
    auto value = std::getenv(variable_name.c_str());
    return value == nullptr ? default_value : value;
}

template<typename T, typename std::enable_if<std::is_arithmetic_v<T>, bool>::type = true>
T GetEnv(const std::string& variable_name, T default_value) {
    auto value = std::getenv(variable_name.c_str());
    if (value == nullptr) {
        return default_value;
    }
    return std::atoi(value);
}

template<typename T, typename std::enable_if<std::is_same_v<T, std::string>, bool>::type = true>
std::optional<T> GetEnv(const std::string& variable_name) {
    auto value = std::getenv(variable_name.c_str());
    return value == nullptr ? std::nullopt : std::make_optional(value);
}

template<typename T, typename std::enable_if<std::is_arithmetic_v<T>, bool>::type = true>
std::optional<T> GetEnv(const std::string& variable_name) {
    auto value = std::getenv(variable_name.c_str());
    if (value == nullptr) {
        return std::nullopt;
    }
    return std::atoi(value);
}

} // namespace join_server::utils
