#pragma once

#include <sstream>
#include <iomanip>
#include "read_stream.hpp"
#include "fwd.hpp"

namespace putils {
    template<typename Obj>
    std::string to_string(Obj && obj) {
        std::stringstream s;
        s << FWD(obj);
        return s.str();
    }

    inline std::string to_string(bool v) {
        std::stringstream s;
        s << std::boolalpha << v;
        return s.str();
    }

    inline std::string to_string(std::istream & s) {
        return read_stream(s);
    }

    template<typename Obj>
    Obj parse(std::string_view str) {
        std::stringstream s(FWD(str));

        Obj ret;
        s >> ret;
        return ret;
    };

    template<>
    inline bool parse(std::string_view str) { return str == "true"; }
}