//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "views.hpp"
#include "concepts.hpp"
#include "utils.hpp"
#include <vector>
#include <sstream>

namespace concepts {
    template<typename TOutput, typename TFunc, typename... TInput>
    concept TransformFunctor =
            requires
            {
                requires (concepts::View<TInput> && ...);
                requires concepts::MutableView<TOutput>;
                requires std::is_invocable_r_v<typename std::decay_t<TOutput>::value_type, TFunc, typename std::decay_t<
                    TInput>::value_type...>;
            };
}


auto get_magic_const_buffer_one() -> concepts::ConstView auto {
    std::vector<int> magic_one = {72, 101, 108, 108, 111, 32, 72, 101, 108, 108, 111};
    return ManagedView<int const>{magic_one.size(), [magic_one](auto index) { return magic_one[index]; }};
}

auto get_magic_const_buffer_two() -> concepts::ConstView auto {
    std::vector<int> magic_two = {0, 0, 0, 0, 0, 0, 15, 10, 6, 0, -11};
    return UniqueManagedView<int const>{magic_two.size(), [magic_two](auto index) { return magic_two[index]; }};
}

void transform(concepts::MutableView auto &output, auto &&functor, concepts::View auto const &... inputs)
    requires concepts::TransformFunctor<ALPAKA_TYPEOF_t(output), ALPAKA_TYPEOF_t(functor), ALPAKA_TYPEOF_t(inputs)...> {
    for (auto i = 0; i < output.size(); i++) {
        output[i] = functor((inputs[i])...);
    }
}


TEST_CASE("Return value concepts.", "") {
    concepts::ConstView auto magic_one = get_magic_const_buffer_one();

    // dangerous but possible
    // the interface only guaranties that you get a view which is immutable
    UniqueManagedView magic_two = get_magic_const_buffer_two();

    UniqueManagedView<int> result(magic_one.size());

    transform(result, std::plus<int>{}, magic_one, magic_two);

    std::stringstream ss;

    for (auto i = 0; i < magic_one.size(); i++) {
        ss << static_cast<char>(result[i]);
    }

    std::string const s = ss.str();
    INFO(s);
    REQUIRE(s == "Hello World");
}
