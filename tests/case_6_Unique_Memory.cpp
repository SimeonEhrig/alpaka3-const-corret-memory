//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "views.hpp"

TEST_CASE("Move unqiue Memory", "") {
    UniqueManagedView<int> mem(1, [](auto) { return 42; });
    REQUIRE(mem[0] == 42);
    REQUIRE(mem.is_valid());
    REQUIRE(mem.data() != nullptr);

    int *ptr = mem.data();
    UniqueManagedView<int> mem2(std::move(mem));

    REQUIRE_FALSE(mem.is_valid());
    REQUIRE(mem.data() == nullptr);
    REQUIRE(mem2[0] == 42);
    REQUIRE(mem2.is_valid());
    REQUIRE(mem2.data() == ptr);
}
