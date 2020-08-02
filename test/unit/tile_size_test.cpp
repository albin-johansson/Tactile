#include "tile_size.hpp"

#include <catch.hpp>

using namespace tactile;

TEST_CASE("TileSize::reset", "[TileSize]")
{
  tile_size size;

  size.set_size(74);
  size.reset();

  CHECK(size.get() == tile_size::default_size());
}

TEST_CASE("TileSize::increase", "[TileSize]")
{
  tile_size size;

  size.set_size(100);
  size.increase();

  CHECK(size.get() == 110);  // should increase by 10 percent
}

TEST_CASE("TileSize::decrease", "[TileSize]")
{
  tile_size size;

  size.set_size(100);
  size.decrease();

  CHECK(size.get() == 90);  // should decrease by 10 percent

  for (int i = 0; i < 100; ++i) {
    size.decrease();
  }

  CHECK(size.get() == tile_size::min_size());
}

TEST_CASE("TileSize::set_size", "[TileSize]")
{
  tile_size tileSize;

  SECTION("Valid size")
  {
    const auto newSize = 83;
    tileSize.set_size(newSize);

    CHECK(tileSize.get() == newSize);
  }

  SECTION("Check that minimum size is enforced")
  {
    const auto badSize = tile_size::min_size() - 1;
    tileSize.set_size(badSize);

    CHECK(tileSize.get() == tile_size::min_size());
  }
}