//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "views.hpp"
#include "concepts.hpp"
#include "utils.hpp"


template<concepts::View TInput, concepts::MutableView TOutput, concepts::MutableView TTmpMem = UniqueManagedView<
    std::decay_t<typename TOutput::value_type> > >
void reduce(TInput const &input, TOutput &output,
            TTmpMem &&tmp = UniqueManagedView<std::decay_t<typename TOutput::value_type> >{0}) {
    static_assert(has_same_raw_type<TInput, TOutput>);
    static_assert(has_same_raw_type<TOutput, TTmpMem>);
    for (std::size_t i = 0; i < input.size(); ++i) {
        tmp[0] += input[i];
    }
    output[0] = tmp[0];
}


TEST_CASE("reduce() with ManagedView and tmp memory", "") {
    constexpr std::size_t length = 10;
    ManagedView<int> input(length, [](auto index) { return index + 1; });
    ManagedView<int> tmp(1, [](auto) { return 0; });

    ManagedView<int> output(1, [](auto) { return 0; });
    reduce(input, output, tmp);
    INFO("output: " << output[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output[0] == (length * (length + 1)) / 2);

    ManagedView<int> output2(1, [](auto) { return 0; });
    reduce(input, output2);
    INFO("output2: " << output2[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output2[0] == (length * (length + 1)) / 2);
}

TEST_CASE("reduce() with UniqueMemory and tmp memory", "") {
    constexpr std::size_t length = 10;
    UniqueManagedView<int const> input(length, [](auto index) { return index + 1; });
    UniqueManagedView<int> output(1, [](auto) { return 0; });
    reduce(input, output, UniqueManagedView<int>(0));
    REQUIRE(output[0] == (length * (length + 1)) / 2);

    ManagedView<int> output2(1, [](auto) { return 0; });
    UniqueManagedView<int> tmp(1);
    reduce(input, output2, std::move(tmp));
    INFO("output2: " << output2[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output2[0] == (length * (length + 1)) / 2);
}
