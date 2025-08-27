#pragma once

#include <type_traits>
#include "views.hpp"

namespace details {
    template<typename TDerived, template <typename...>class TBase>
    inline constexpr bool isSpecializationOfBaseOf_v = std::false_type{};

    template<template <typename...>class TDerived, template <typename...>class TBase, typename... TArgs>
        requires(std::derived_from<TDerived<TArgs...>, TBase<TArgs...> >)
    inline constexpr bool isSpecializationOfBaseOf_v<TDerived<TArgs...>, TBase> = std::true_type{};
}

namespace concepts {
    template<typename T>
    concept Memory = requires { requires details::isSpecializationOfBaseOf_v<T, ::Memory>; };

    template<typename T>
    concept View = requires(T t)
    {
        typename std::decay_t<T>::value_type;
        t[0];
        t.size();
    };

    template<typename T>
    concept MutableView = requires(T t)
    {
        requires View<std::decay_t<T> >;
        requires !std::is_const_v<typename std::remove_reference_t<T>::value_type>;
    };

    template<typename T>
    concept ConstView = requires(T t)
    {
        requires View<std::decay_t<T> >;
        requires std::is_const_v<typename std::remove_reference_t<T>::value_type>;
    };
}
