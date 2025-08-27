//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "views.hpp"
#include "concepts.hpp"

template<concepts::View TInput, concepts::MutableView TOutput, typename TTmpMem>
void reduce(TInput const &input, TOutput &output, TTmpMem &tmp) {
    static_assert(
        std::is_same_v<std::decay_t<typename TInput::value_type>, std::decay_t<typename TOutput::value_type> >);
    //static_assert(std::is_same_v<std::decay_t<typename TInput::value_type>, typename TTmpMem::value_type>);
    for (std::size_t i = 0; i < input.size(); ++i) {
        tmp[0] += input[i];
    }
    output[0] = tmp[0];
}

// TODO: replace overload by universal reference
void reduce(concepts::View auto const &input, concepts::MutableView auto &output) {
    ManagedView<int> tmp(1, [](auto) { return 0; });
    reduce(input, output, tmp);
}

TEST_CASE("reduce() with ManagedView and tmp memory", "") {
    constexpr std::size_t length = 10;
    ManagedView<int> intput(length, [](auto index) { return index + 1; });
    ManagedView<int> tmp(1, [](auto) { return 0; });

    ManagedView<int> output(1, [](auto) { return 0; });
    reduce(intput, output, tmp);
    INFO("output: " << output[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output[0] == (length * (length + 1)) / 2);

    ManagedView<int> output2(1, [](auto) { return 0; });
    reduce(intput, output2);
    INFO("output2: " << output2[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output2[0] == (length * (length + 1)) / 2);
}

TEST_CASE("reduce() with UniqueMemory and tmp memory", "") {
    constexpr std::size_t length = 10;
    UniqueManagedView<int const> input(length, [](auto index) { return index + 1; });
    UniqueManagedView<int> output(1, [](auto) { return 0; });
    reduce(input, output);
    REQUIRE(output[0] == (length * (length + 1)) / 2);
}
