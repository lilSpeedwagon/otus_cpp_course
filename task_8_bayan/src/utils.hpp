#pragma once

#include <boost/filesystem/path.hpp>


namespace std {

template<>
struct hash<boost::filesystem::path> {
public:
    size_t operator()(const boost::filesystem::path& path) const {
        return std::hash<std::string>()(path.c_str());
    }
};

} // namespace std

namespace bayan::utils {

class ScopeGuarg {
public:
    using CallbackT = std::function<void(void)>;

    ScopeGuarg(CallbackT callback) : callback_(callback) {}
    ~ScopeGuarg() {
        callback_();
    }

private:
    CallbackT callback_;
};

} // namespace bayan::utils
