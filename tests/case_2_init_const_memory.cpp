//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "mem.hpp"

TEST_CASE("Init Const Memory", "") {
  std::size_t const size = 10;

  SharedContainer<int const> const_v(
    size, [](auto index) { return static_cast<int>(index * 3); });

  auto init_func = [](auto index) { return static_cast<int>(index * 10); };
  SharedContainer<int const> const_v2(size, init_func);

  for (std::size_t i = 0; i < size; ++i) {
    REQUIRE(const_v[i] == static_cast<int>(i) * 3);
  }

  for (std::size_t i = 0; i < size; ++i) {
    REQUIRE(const_v2[i] == static_cast<int>(i) * 10);
  }
}
