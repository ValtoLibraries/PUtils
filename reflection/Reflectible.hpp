#pragma once

#include <atomic>
#include <string>
#include <string_view>

#include "meta/table.hpp"
#include "meta/nameof.hpp"

#define pmeta_reflectible_attribute(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).find("::") + 2), memberPtr
#define pmeta_reflectible_attribute_private(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).find("::") + 3), memberPtr
#define pmeta_reflectible_parent(T) pmeta_nameof(T), pmeta::type<T>()

namespace putils
{
    template<typename CRTP>
    struct Reflectible
    {
    public:
        Reflectible()
        {
            // A reflectible type T should inherit from Reflectible<T> and define
            // `get_attributes`, `get_methods` and `get_parents` static member functions
            // that return a pmeta::table mapping strings to:
            //          `get_attributes`: member pointers to the attributes
            //          `get_methods`: member pointers to the methods
            //          `get_parents`: pmeta::type objects for each parent class

            CRTP::get_class_name();
            CRTP::get_attributes();
            CRTP::get_methods();
            CRTP::get_parents();
        }
    };

    template<typename T, typename = int, typename = int, typename = int, typename = int>
    struct is_reflectible : std::false_type {};

    template<typename T>
    struct is_reflectible<T,
            decltype((void)T::get_class_name(), 0),
            decltype((void)T::get_attributes(), 0),
            decltype((void)T::get_methods(), 0),
            decltype((void)T::get_parents(), 0)
    >
            : std::true_type {};
}
