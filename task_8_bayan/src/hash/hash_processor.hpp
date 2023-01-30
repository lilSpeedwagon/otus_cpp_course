#pragma once


namespace bayan::hash {

class HashProcessor {
public:
    HashProcessor() {}
    virtual ~HashProcessor() {}

    virtual size_t Hash(const std::string& data) const = 0;
};

// TODO
// create a factory returning a poitner to the same hasher every time ?
// lazy init with some hash type string

} // namespace bayan::hash
