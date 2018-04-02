#pragma once

namespace putils {
    template<typename S, typename T>
    class is_streamable {
        template<typename SS, typename TT>
        static auto test(int)
        -> decltype(std::declval<SS &>() << std::declval<TT>(), std::true_type());

        template<typename, typename>
        static auto test(...) -> std::false_type;

    public:
        static const bool value = decltype(test<S, T>(0))::value;
    };

    template<typename S, typename T>
    class is_unstreamable {
        template<typename SS, typename TT>
        static auto test(int)
        -> decltype(std::declval<SS &>() >> std::declval<TT &>(), std::true_type());

        template<typename, typename>
        static auto test(...) -> std::false_type;

    public:
        static const bool value = decltype(test<S, T>(0))::value;
    };
}