//
// Copyright 2025 Simeon Ehrig
// SPDX-License-Identifier: ISC
//

#include <catch2/catch_test_macros.hpp>

#include "mem.hpp"

TEST_CASE("Access mutable memory", "") {
  std::size_t size = 10;
  SharedBuffer<int> v(size);
  INFO("[before] v: " << v << "\n");

  for (std::size_t i = 0; i < v.size(); ++i) {
    v[i] = static_cast<int>(i) + 2;
  }

  INFO("[after] v: " << v << "\n");

  for (std::size_t i = 0; i < size; ++i) {
    REQUIRE(v[i] == static_cast<int>(i) + 2);
  }
}
