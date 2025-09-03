//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include "concepts.hpp"
#include "mem.hpp"
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
    STATIC_REQUIRE(concepts::Memory<SharedBuffer<int>>);
    STATIC_REQUIRE(!concepts::Memory<Foo<int>>);
}

using ViewElementTypes = std::tuple<int, int const, int &, int const &>;

TEMPLATE_LIST_TEST_CASE("Test concepts::View", "", ViewElementTypes) {
    STATIC_REQUIRE(!concepts::View<Memory<TestType>>);
    STATIC_REQUIRE(concepts::View<SharedBuffer<TestType>>);
    STATIC_REQUIRE(concepts::View<UniqueBuffer<TestType>>);
}


TEST_CASE("Test concepts::MutableView", "") {
    STATIC_REQUIRE(concepts::MutableView<SharedBuffer<int>>);
    STATIC_REQUIRE(concepts::MutableView<SharedBuffer<int&>>);
    STATIC_REQUIRE(!concepts::MutableView<SharedBuffer<int const>>);
    STATIC_REQUIRE(!concepts::MutableView<SharedBuffer<int const &>>);
    STATIC_REQUIRE(!concepts::MutableView<SharedBuffer<int> const>);
    STATIC_REQUIRE(!concepts::MutableView<SharedBuffer<int const> const>);
    STATIC_REQUIRE(!concepts::MutableView<SharedBuffer<int const> const&>);

    STATIC_REQUIRE(concepts::MutableView<UniqueBuffer<int>>);
    STATIC_REQUIRE(concepts::MutableView<UniqueBuffer<int&>>);
    STATIC_REQUIRE(!concepts::MutableView<UniqueBuffer<int const>>);
    STATIC_REQUIRE(!concepts::MutableView<UniqueBuffer<int const &>>);
}

TEST_CASE("Test concepts::ConstView", "") {
    STATIC_REQUIRE(!concepts::ConstView<SharedBuffer<int>>);
    STATIC_REQUIRE(!concepts::ConstView<SharedBuffer<int&>>);
    STATIC_REQUIRE(concepts::ConstView<SharedBuffer<int const>>);
    STATIC_REQUIRE(concepts::ConstView<SharedBuffer<int const &>>);

    STATIC_REQUIRE(!concepts::ConstView<UniqueBuffer<int>>);
    STATIC_REQUIRE(!concepts::ConstView<UniqueBuffer<int&>>);
    STATIC_REQUIRE(concepts::ConstView<UniqueBuffer<int const>>);
    STATIC_REQUIRE(concepts::ConstView<UniqueBuffer<int const &>>);
}

using ConstructableElementTypes = std::tuple<int, int const>;

TEMPLATE_LIST_TEST_CASE(
    "ManagedView constructors and assignment operators", "", ConstructableElementTypes
) {
    STATIC_REQUIRE(!std::is_default_constructible_v<SharedBuffer<TestType>>);
    STATIC_REQUIRE(std::is_copy_constructible_v<SharedBuffer<TestType>>);
    STATIC_REQUIRE(std::is_copy_assignable_v<SharedBuffer<TestType>>);
    STATIC_REQUIRE(!std::is_move_constructible_v<SharedBuffer<TestType>>);
    STATIC_REQUIRE(!std::is_move_assignable_v<SharedBuffer<TestType>>);
}

TEMPLATE_LIST_TEST_CASE(
    "UniqueManagedView constructors and assignment operators", "", ConstructableElementTypes
) {
    STATIC_REQUIRE(!std::is_default_constructible_v<UniqueBuffer<TestType>>);
    STATIC_REQUIRE(!std::is_copy_constructible_v<UniqueBuffer<TestType>>);
    STATIC_REQUIRE(!std::is_copy_assignable_v<UniqueBuffer<TestType>>);
    STATIC_REQUIRE(std::is_move_constructible_v<UniqueBuffer<TestType>>);
    STATIC_REQUIRE(std::is_move_assignable_v<UniqueBuffer<TestType>>);
}
