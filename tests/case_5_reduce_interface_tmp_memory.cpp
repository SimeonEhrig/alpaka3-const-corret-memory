//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "views.hpp"
#include "concepts.hpp"
#include "utils.hpp"

namespace concepts {
    template<typename TFunc, typename TInput, typename TOutput>
    concept ReduceFunctor =
            requires
            {
                requires concepts::View<TInput>;
                requires concepts::MutableView<TOutput>;
                requires std::is_invocable_r_v<typename std::decay_t<TInput>::value_type, TFunc, typename
                    std::decay_t<TOutput>::value_type, typename std::decay_t<TInput>::value_type>;
            };
}

template<concepts::View TInput, concepts::MutableView TOutput, typename TFunc, concepts::MutableView TTmpMem =
    UniqueManagedView<
        std::decay_t<typename TOutput::value_type> > >
    requires concepts::ReduceFunctor<TFunc, TInput, TOutput>
void reduce(TInput const &input, TOutput &output,
            TFunc &&functor,
            TTmpMem &&tmp = UniqueManagedView<std::decay_t<typename TOutput::value_type> >{0}) {
    static_assert(has_same_raw_type<TOutput, TTmpMem>);
    for (std::size_t i = 0; i < input.size(); ++i) {
        tmp[0] = functor(tmp[0], input[i]);
    }
    output[0] = tmp[0];
}

// Same functionality like reduce(), but use auto keyword for the arguments instead typenames.
// Only a demonstrator how a function interface can be also defined.
void reduce2(concepts::View auto const &input, concepts::MutableView auto &output, auto &&functor,
             concepts::MutableView auto &&tmp = UniqueManagedView<ALPAKA_TYPEOF_t(output)::value_type>{0}
) requires concepts::ReduceFunctor<ALPAKA_TYPEOF_t(functor), ALPAKA_TYPEOF_t(input), ALPAKA_TYPEOF_t(output) > {
    static_assert(has_same_raw_type<ALPAKA_TYPEOF(output), ALPAKA_TYPEOF(tmp) >);
    for (std::size_t i = 0; i < input.size(); ++i) {
        tmp[0] = functor(tmp[0], input[i]);
    }
    output[0] = tmp[0];
}


TEST_CASE("reduce() with ManagedView and tmp memory", "") {
    constexpr std::size_t length = 10;
    ManagedView<int> input(length, [](auto index) { return index + 1; });
    ManagedView<int> tmp(1, [](auto) { return 0; });

    ManagedView<int> output(1, [](auto) { return 0; });
    reduce(input, output, std::plus<int>{}, tmp);
    INFO("output: " << output[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output[0] == (length * (length + 1)) / 2);

    ManagedView<double> input_double(length, [](auto index) { return static_cast<double>(index) + 1.0; });
    ManagedView<int> output2(1, [](auto) { return 0; });
    reduce(input_double, output2, []<typename TResult>(TResult result, auto input) {
               return result + static_cast<TResult>(input);
           }
    );
    INFO("output2: " << output2[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output2[0] == (length * (length + 1)) / 2);
}

TEST_CASE("reduce() with UniqueMemory and tmp memory", "") {
    constexpr std::size_t length = 10;
    UniqueManagedView<int const> input(length, [](auto index) { return index + 1; });
    UniqueManagedView<int> output(1, [](auto) { return 0; });
    auto functor = [](auto result, auto input) {
        return result + input;
    };
    reduce(input, output, functor, UniqueManagedView<int>(0));
    REQUIRE(output[0] == (length * (length + 1)) / 2);

    ManagedView<int> output2(1, [](auto) { return 0; });
    UniqueManagedView<int> tmp(1);
    reduce(input, output2, std::plus<int>{}, std::move(tmp));
    INFO("output2: " << output2[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output2[0] == (length * (length + 1)) / 2);
}

TEST_CASE("reduce2() with UniqueMemory and tmp memory", "") {
    constexpr std::size_t length = 10;
    UniqueManagedView<int const> const input(length, [](auto index) { return index + 1; });

    ManagedView<int> output(1, [](auto) { return 0; });
    UniqueManagedView<int> tmp(1);
    reduce2(input, output, std::plus<int>{}, std::move(tmp));
    INFO("output: " << output[0] << " (expected: " << (length * (length + 1)) / 2 << ")\n");
    REQUIRE(output[0] == (length * (length + 1)) / 2);
}
