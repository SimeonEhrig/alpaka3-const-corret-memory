//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#pragma once

#include "concepts.hpp"

#define ALPAKA_TYPEOF(...) std::decay_t<decltype(__VA_ARGS__)>

// take two views and compare the value_type without qualifiers like const
template<concepts::View ViewA, concepts::View ViewB>
constexpr bool has_same_raw_type = std::is_same_v<std::decay_t<typename ViewA::value_type>, std::decay_t<typename
    std::decay_t<
        ViewB>::value_type> >;
