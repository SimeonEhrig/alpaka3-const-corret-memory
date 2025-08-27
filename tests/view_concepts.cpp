//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include "concepts.hpp"
#include "views.hpp"
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <utility>
#include <type_traits>

template<typename T>
class Foo {
};

TEST_CASE("Test View concepts", "") {
    STATIC_REQUIRE(concepts::Memory<Memory<int>>);
    STATIC_REQUIRE(concepts::Memory<ManagedView<int>>);
    STATIC_REQUIRE(!concepts::Memory<Foo<int>>);

    STATIC_REQUIRE(!concepts::View<Memory<int>>);
    STATIC_REQUIRE(concepts::View<ManagedView<int>>);
    STATIC_REQUIRE(concepts::View<UniqueManagedView<int>>);

    STATIC_REQUIRE(concepts::MutableView<ManagedView<int>>);
    STATIC_REQUIRE(!concepts::ConstView<ManagedView<int>>);

    STATIC_REQUIRE(!concepts::MutableView<ManagedView<int const>>);
    STATIC_REQUIRE(concepts::ConstView<ManagedView<int const>>);
}

using ViewElementTypes = std::tuple<int, int const>;

TEMPLATE_LIST_TEST_CASE(
    "ManagedView constructors and assignment operators", "", ViewElementTypes
) {
    STATIC_REQUIRE(!std::is_default_constructible_v<ManagedView<TestType>>);
    STATIC_REQUIRE(std::is_copy_constructible_v<ManagedView<TestType>>);
    STATIC_REQUIRE(std::is_copy_assignable_v<ManagedView<TestType>>);
    STATIC_REQUIRE(!std::is_move_constructible_v<ManagedView<TestType>>);
    STATIC_REQUIRE(!std::is_move_assignable_v<ManagedView<TestType>>);
}

TEMPLATE_LIST_TEST_CASE(
    "UniqueManagedView constructors and assignment operators", "", ViewElementTypes
) {
    STATIC_REQUIRE(!std::is_default_constructible_v<UniqueManagedView<TestType>>);
    STATIC_REQUIRE(!std::is_copy_constructible_v<UniqueManagedView<TestType>>);
    STATIC_REQUIRE(!std::is_copy_assignable_v<UniqueManagedView<TestType>>);
    STATIC_REQUIRE(std::is_move_constructible_v<UniqueManagedView<TestType>>);
    STATIC_REQUIRE(std::is_move_assignable_v<UniqueManagedView<TestType>>);
}
