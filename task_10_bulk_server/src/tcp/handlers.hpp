#pragma once

#include <functional>
#include <string>


namespace bulk_server::tcp {

using DataHandler = std::function<void(std::string)>;

} // namespace bulk_server::tcp
