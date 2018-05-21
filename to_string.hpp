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
    void parse(Obj & obj, const std::string & str) {
        std::stringstream s(FWD(str));
        s >> obj;
    };

    template<>
    inline void parse(bool & obj, const std::string & str) {
		obj = (str == "true");
    };

	template<>
	inline void parse(std::string & obj, const std::string & str) {
		obj = str;
	}

    template<typename Obj>
    Obj parse(const std::string & str) {
        std::stringstream s(FWD(str));

        Obj ret;
        s >> ret;
        return ret;
    };

    template<>
    inline bool parse(const std::string & str) { return str == "true"; }

    template<>
    inline std::string parse(const std::string & str) { return str; }
}