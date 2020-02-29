#include "flood_fill.h"

#include <catch.hpp>
#include <iostream>

using namespace tactile;

TEST_CASE("Correctness of flood fill algorithm", "[FloodFill]")
{
  TileLayer layer{5, 5};
  for (int i = 0; i < 5; ++i) {
    layer.set_tile({i, i}, 1);
  }

  /*
   1 0 0 0 0
   0 1 0 0 0
   0 0 1 0 0
   0 0 0 1 0
   0 0 0 0 1
   */

  const auto id = 2;
  layer.flood({1, 0}, 0, id);

  /*
   Expected:
   1 0 0 0 0
   2 1 0 0 0
   2 2 1 0 0
   2 2 2 1 0
   2 2 2 2 1
   */

  SECTION("Affected tiles")
  {
    CHECK(*layer.tile_at({1, 0}) == id);
    CHECK(*layer.tile_at({2, 0}) == id);
    CHECK(*layer.tile_at({3, 0}) == id);
    CHECK(*layer.tile_at({4, 0}) == id);

    CHECK(*layer.tile_at({2, 1}) == id);
    CHECK(*layer.tile_at({3, 1}) == id);
    CHECK(*layer.tile_at({4, 1}) == id);

    CHECK(*layer.tile_at({3, 2}) == id);
    CHECK(*layer.tile_at({4, 2}) == id);

    CHECK(*layer.tile_at({4, 3}) == id);
  }

  SECTION("Unaffected diagonal blocking the flood")
  {
    CHECK(*layer.tile_at({0, 0}) == 1);
    CHECK(*layer.tile_at({1, 1}) == 1);
    CHECK(*layer.tile_at({2, 2}) == 1);
    CHECK(*layer.tile_at({3, 3}) == 1);
    CHECK(*layer.tile_at({4, 4}) == 1);
  }

  SECTION("Unaffected right hand side")
  {
    CHECK(*layer.tile_at({0, 1}) == empty);
    CHECK(*layer.tile_at({0, 2}) == empty);
    CHECK(*layer.tile_at({0, 3}) == empty);
    CHECK(*layer.tile_at({0, 4}) == empty);

    CHECK(*layer.tile_at({1, 2}) == empty);
    CHECK(*layer.tile_at({1, 3}) == empty);
    CHECK(*layer.tile_at({1, 4}) == empty);

    CHECK(*layer.tile_at({2, 3}) == empty);
    CHECK(*layer.tile_at({2, 4}) == empty);

    CHECK(*layer.tile_at({3, 4}) == empty);
  }
}

TEST_CASE("Out-of-bounds position", "[FloodFill]")
{
  TileLayer layer{5, 5};
  CHECK_NOTHROW(layer.flood({5, 5}, 0, 0));
  CHECK_NOTHROW(layer.flood({6, 6}, 0, 0));
  CHECK_NOTHROW(flood_fill(layer, {6, 6}, 0, 0));
}