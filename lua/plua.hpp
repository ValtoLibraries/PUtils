#pragma once

#include "sol.hpp"
#include "traits.hpp"

namespace putils
{
    namespace lua
    {
        template<typename T>
        void registerType(sol::state &state)
        {
            static_assert(putils::is_reflectible<T>::value, "Please make your type reflectible before registering it");

            const auto t = std::tuple_cat(
                    std::make_tuple(T::get_class_name()),
                    T::get_attributes().getFlatKeyValues(),
                    T::get_methods().getFlatKeyValues()
            );

            std::apply(
                    [&state](auto &&...params) { state.new_usertype<T>(FWD(params)...); },
                    t
            );

            if constexpr (putils::is_streamable<std::ostream, T>::value)
            {
                state[T::get_class_name()][sol::meta_function::to_string] =
                        [](const T &obj) { return putils::to_string(obj); };
            }
        }
    }
}