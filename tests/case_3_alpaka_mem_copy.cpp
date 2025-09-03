//
// Created by simeon on 27.08.25.
//
//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "mem.hpp"
#include "concepts.hpp"

namespace alpaka {
    void memcpy(concepts::View auto &dest, concepts::View auto const &src) {
        assert(dest.size() >= src.size());
        for (std::size_t i = 0; i < src.size(); ++i) {
            dest[i] = src[i];
        }
    }
}

TEST_CASE("alpaka::memcpy", "") {
    constexpr std::size_t size = 10;
    SharedBuffer<int> output(size);
    INFO("[before] output:              " << output << "\n");

    SharedCollection<int> mutable_input(size, [](auto) { return 1; });
    alpaka::memcpy(output, mutable_input);
    INFO("[copy mutable memory] output: " << output << "\n");

    for (auto i = 0; i < size; ++i) {
        REQUIRE(mutable_input[i] == output[i]);
    }

    SharedCollection<int const> const_input(size, [](auto) { return 2; });
    alpaka::memcpy(output, const_input);
    INFO("[copy const memory] output:   " << output << "\n");

    for (auto i = 0; i < size; ++i) {
        REQUIRE(const_input[i] == output[i]);
    }
}
