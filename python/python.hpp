#pragma once

#include "pybind11/stl.h"
#include "pybind11/functional.h"
#include "pybind11/chrono.h"
#include "pybind11/cast.h"
#include "pybind11/iostream.h"
#include "pybind11/embed.h"
#include "traits.hpp"
#include "reflection/Reflectible.hpp"

namespace py = pybind11;

namespace putils {
    namespace python {
        template<typename T>
        void registerType(py::module & m) {
            static_assert(putils::is_reflectible<T>::value, "Please make your type reflectible before registering it");

            py::class_<T> type(m, T::get_class_name(), py::dynamic_attr());
            pmeta::tuple_for_each(T::get_attributes().getKeyValues(), [&type](auto && p) {
                using MemberType = decltype(std::declval<T>().*(p.second));
                if constexpr (std::is_assignable<MemberType, MemberType>::value)
                    type.def_readwrite(p.first.data(), p.second);
                else
                    type.def_readonly(p.first.data(), p.second);
            });

            pmeta::tuple_for_each(T::get_methods().getKeyValues(), [&type](auto && p) {
                type.def(p.first.data(), p.second);
            });

            if constexpr (putils::is_streamable<std::ostream, T>::value)
                type.def("__str__", [](const T & obj) { return putils::to_string(obj); });
        }
    }
}
