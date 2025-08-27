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

TEST_CASE("Test concepts::Memory", "") {
    STATIC_REQUIRE(concepts::Memory<Memory<int>>);
    STATIC_REQUIRE(concepts::Memory<ManagedView<int>>);
    STATIC_REQUIRE(!concepts::Memory<Foo<int>>);
}

using ViewElementTypes = std::tuple<int, int const, int &, int const &>;

TEMPLATE_LIST_TEST_CASE("Test concepts::View", "", ViewElementTypes) {
    STATIC_REQUIRE(!concepts::View<Memory<TestType>>);
    STATIC_REQUIRE(concepts::View<ManagedView<TestType>>);
    STATIC_REQUIRE(concepts::View<UniqueManagedView<TestType>>);
}


TEST_CASE("Test concepts::MutableView", "") {
    STATIC_REQUIRE(concepts::MutableView<ManagedView<int>>);
    STATIC_REQUIRE(concepts::MutableView<ManagedView<int&>>);
    STATIC_REQUIRE(!concepts::MutableView<ManagedView<int const>>);
    STATIC_REQUIRE(!concepts::MutableView<ManagedView<int const &>>);

    STATIC_REQUIRE(concepts::MutableView<UniqueManagedView<int>>);
    STATIC_REQUIRE(concepts::MutableView<UniqueManagedView<int&>>);
    STATIC_REQUIRE(!concepts::MutableView<UniqueManagedView<int const>>);
    STATIC_REQUIRE(!concepts::MutableView<UniqueManagedView<int const &>>);
}

TEST_CASE("Test concepts::ConstView", "") {
    STATIC_REQUIRE(!concepts::ConstView<ManagedView<int>>);
    STATIC_REQUIRE(!concepts::ConstView<ManagedView<int&>>);
    STATIC_REQUIRE(concepts::ConstView<ManagedView<int const>>);
    STATIC_REQUIRE(concepts::ConstView<ManagedView<int const &>>);

    STATIC_REQUIRE(!concepts::ConstView<UniqueManagedView<int>>);
    STATIC_REQUIRE(!concepts::ConstView<UniqueManagedView<int&>>);
    STATIC_REQUIRE(concepts::ConstView<UniqueManagedView<int const>>);
    STATIC_REQUIRE(concepts::ConstView<UniqueManagedView<int const &>>);
}

using ConstructableElementTypes = std::tuple<int, int const>;

TEMPLATE_LIST_TEST_CASE(
    "ManagedView constructors and assignment operators", "", ConstructableElementTypes
) {
    STATIC_REQUIRE(!std::is_default_constructible_v<ManagedView<TestType>>);
    STATIC_REQUIRE(std::is_copy_constructible_v<ManagedView<TestType>>);
    STATIC_REQUIRE(std::is_copy_assignable_v<ManagedView<TestType>>);
    STATIC_REQUIRE(!std::is_move_constructible_v<ManagedView<TestType>>);
    STATIC_REQUIRE(!std::is_move_assignable_v<ManagedView<TestType>>);
}

TEMPLATE_LIST_TEST_CASE(
    "UniqueManagedView constructors and assignment operators", "", ConstructableElementTypes
) {
    STATIC_REQUIRE(!std::is_default_constructible_v<UniqueManagedView<TestType>>);
    STATIC_REQUIRE(!std::is_copy_constructible_v<UniqueManagedView<TestType>>);
    STATIC_REQUIRE(!std::is_copy_assignable_v<UniqueManagedView<TestType>>);
    STATIC_REQUIRE(std::is_move_constructible_v<UniqueManagedView<TestType>>);
    STATIC_REQUIRE(std::is_move_assignable_v<UniqueManagedView<TestType>>);
}
