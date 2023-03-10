#pragma once

#include <functional>
#include <string>


namespace join_server::tcp {

using DataHandler = std::function<std::string(std::string)>;

} // namespace join_server::tcp
