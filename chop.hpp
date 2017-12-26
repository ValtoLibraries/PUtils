#pragma once

#include <string>
#include <string_view>
#include "runTests.hpp"

namespace putils {
    inline std::string chop(std::string_view str) {
        return std::string(str.substr(0, str.find_last_not_of(" \t\n\r") + 1));
    }
}
