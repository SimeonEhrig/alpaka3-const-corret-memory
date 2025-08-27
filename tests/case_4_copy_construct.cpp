//
// Created by simeon on 27.08.25.
//
//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "views.hpp"
#include <concepts>

TEST_CASE("copy constructor", "") {
    ManagedView<int> m1(2, [](auto index) { return static_cast<int>(index); });
    INFO("m1: " << m1 << "\n");

    ManagedView<int> m2(m1);
    INFO("m2: " << m2 << "\n");

    REQUIRE(m1[0] == m2[0]);
    REQUIRE(m1[1] == m2[1]);


    m1[0] = 42;
    INFO("m1 <> m2: " << m1 << " <> " << m2 << "\n");
    REQUIRE(m1[0] == m2[0]);


    ManagedView<int const> c1(2, [](auto) { return static_cast<int>(7); });
    ManagedView<int const> c2(c1);
    INFO("c1 <> c2: " << c1 << " <> " << c2 << "\n");
    REQUIRE(c1[0] == c2[0]);
    REQUIRE(c1[0] == c2[0]);


    ManagedView<int> m3(2, [](auto) { return static_cast<int>(3); });
    ManagedView<int const> c3(m3);

    INFO("m3 <> c3: " << m3 << " <> " << c3 << "\n");
    m3[0] = 18;
    INFO("m3 <> c3: " << m3 << " <> " << c3 << "\n");

    REQUIRE(m3[0] == c3[0]);
    REQUIRE(m3[0] == c3[0]);

    // should be not possible
    //ManagedView<int> m4(c3);

    STATIC_REQUIRE(std::constructible_from<ManagedView<int>, ManagedView<int>&>);
    STATIC_REQUIRE(std::constructible_from<ManagedView<int const>, ManagedView<int const>&>);
    STATIC_REQUIRE(std::constructible_from<ManagedView<int const>, ManagedView<int>&>);
    STATIC_REQUIRE(!std::constructible_from<ManagedView<int>, ManagedView<int const>&>);
}

TEST_CASE("assignment oper", "") {
    ManagedView<int> m1(1, [](auto index) { return static_cast<int>(index + 28); });
    ManagedView<int> m2 = m1;
    REQUIRE(m1[0] == m2[0]);

    ManagedView<int const> c1(1, [](auto) { return static_cast<int>(7); });
    ManagedView<int const> c2 = c1;
    REQUIRE(c1[0] == c2[0]);

    // c1 = m1 is not possible in general, because the assignment operator does not allow to be templated
}
