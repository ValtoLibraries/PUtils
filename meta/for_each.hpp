#pragma once

#include <functional>
#include "type.hpp"
#include "fwd.hpp"

namespace pmeta {
    // Implementation details
    namespace detail {
        template<typename F, typename Tuple, size_t ...Is>
        void tuple_for_each(F && f, Tuple && tuple, std::index_sequence<Is...>);

        template<typename F, typename Tuple>
        void tuple_for_each(F &&, Tuple &&, std::index_sequence<>) {}

        template<typename F, typename Tuple, size_t I, size_t ...Is>
        void tuple_for_each(F && f, Tuple && tuple, std::index_sequence<I, Is...>) {
            f(std::get<I>(tuple));
            detail::tuple_for_each(std::forward<F>(f), tuple, std::index_sequence<Is...>());
        }
    }

    // For each `e` element in `tuple`, call `f(e)`
    // For instance:
    //      std::tuple<int, std::string, int> t(42, "test", 1);
    //      pmeta::tuple_for_each(t, [](auto &attr) { std::cout << attr << std::endl; });
    // Will print
    //      42
    //      test
    //      1
    //
    template<typename F, typename ...Args>
    void tuple_for_each(std::tuple<Args...> & tuple, F && f) {
        detail::tuple_for_each(std::forward<F>(f), tuple, std::index_sequence_for<Args...>());
    }

    template<typename F, typename ...Args>
    void tuple_for_each(const std::tuple<Args...> & tuple, F && f) {
        detail::tuple_for_each(std::forward<F>(f), tuple, std::index_sequence_for<Args...>());
    }
}
