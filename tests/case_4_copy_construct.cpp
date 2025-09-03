//
// Created by simeon on 27.08.25.
//
//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "mem.hpp"
#include <concepts>

TEST_CASE("copy constructor", "") {
    SharedBuffer<int> m1(2);
    for (auto i = 0; i < m1.size(); ++i) {
        m1[i] = i;
    }
    INFO("m1: " << m1 << "\n");

    SharedBuffer<int> m2(m1);
    INFO("m2: " << m2 << "\n");

    REQUIRE(m1[0] == m2[0]);
    REQUIRE(m1[1] == m2[1]);


    m1[0] = 42;
    INFO("m1 <> m2: " << m1 << " <> " << m2 << "\n");
    REQUIRE(m1[0] == m2[0]);


    SharedCollection<int const> c1(2, [](auto) { return static_cast<int>(7); });

    SharedCollection<int const> c2(c1);
    INFO("c1 <> c2: " << c1 << " <> " << c2 << "\n");
    REQUIRE(c1[0] == c2[0]);
    REQUIRE(c1[0] == c2[0]);


    SharedBuffer<int> m3(2);
    for (auto i = 0; i < m3.size(); ++i) {
        m3[i] = 3;
    }
    SharedBuffer<int const> c3(m3);

    INFO("m3 <> c3: " << m3 << " <> " << c3 << "\n");
    m3[0] = 18;
    INFO("m3 <> c3: " << m3 << " <> " << c3 << "\n");

    REQUIRE(m3[0] == c3[0]);
    REQUIRE(m3[0] == c3[0]);

    // should be not possible
    //ManagedView<int> m4(c3);

    STATIC_REQUIRE(std::constructible_from<SharedBuffer<int>, SharedBuffer<int>&>);
    STATIC_REQUIRE(std::constructible_from<SharedBuffer<int const>, SharedBuffer<int const>&>);
    STATIC_REQUIRE(std::constructible_from<SharedBuffer<int const>, SharedBuffer<int>&>);
    STATIC_REQUIRE(!std::constructible_from<SharedBuffer<int>, SharedBuffer<int const>&>);
}

TEST_CASE("assignment oper", "") {
    SharedBuffer<int> m1(1);
    m1[0] = 28;
    SharedBuffer<int> m2 = m1;
    REQUIRE(m1[0] == m2[0]);

    SharedCollection<int const> c1(1, [](auto) { return static_cast<int>(7); });
    SharedCollection<int const> c2 = c1;
    REQUIRE(c1[0] == c2[0]);

    // c1 = m1 is not possible in general, because the assignment operator does not allow to be templated
}
