#include "tileset.hpp"

#include <QImage>
#include <catch.hpp>
#include <memory>

#include "tactile_error.hpp"
#include "tactile_types.hpp"

using namespace tactile;
using namespace tactile::model;

TEST_CASE("tileset(QImage, int, int)", "[tileset]")
{
  SECTION("Null image")
  {
    QImage image;
    CHECK_THROWS_AS(tileset(image, 10, 10), tactile_error);
  }

  SECTION("Zero size") { CHECK_NOTHROW(tileset{"terrain.png", 0, 0}); }

  SECTION("Good args") { CHECK_NOTHROW(tileset{"terrain.png", 32, 32}); }
}

TEST_CASE("tileset::select", "[tileset]")
{
  const auto tileWidth = 32;
  const auto tileHeight = 32;
  tileset sheet{"terrain.png", tileWidth, tileHeight};

  sheet.select(0, 0);
  CHECK(std::distance(sheet.begin(), sheet.end()) == 1);
  CHECK(sheet.num_selected() == 1);

  sheet.select(0, 0);
  CHECK(std::distance(sheet.begin(), sheet.end()) == 1);
  CHECK(sheet.num_selected() == 1);

  sheet.select(3 * tileWidth, 2 * tileHeight);
  CHECK(std::distance(sheet.begin(), sheet.end()) == 2);
  CHECK(sheet.num_selected() == 2);

  sheet.select(-1, -1);
  CHECK(std::distance(sheet.begin(), sheet.end()) == 2);
  CHECK(sheet.num_selected() == 2);

  sheet.clear_selection();
  CHECK(std::distance(sheet.begin(), sheet.end()) == 0);
  CHECK(sheet.num_selected() == 0);
}

TEST_CASE("tileset::set_first_id", "[tileset]")
{
  tileset sheet{"terrain.png", 32, 32};

  CHECK(sheet.first_id() == tile_id{1});

  const tile_id id{84};
  sheet.set_first_id(id);

  CHECK(sheet.first_id() == id);
}

TEST_CASE("tileset::width", "[tileset]")
{
  const QImage image{"terrain.png"};
  tileset sheet{image, 32, 32};

  CHECK(sheet.width() == image.width());
}

TEST_CASE("tileset::height", "[tileset]")
{
  const QImage image{"terrain.png"};
  tileset sheet{image, 32, 32};

  CHECK(sheet.height() == image.height());
}

TEST_CASE("tileset::tiles", "[tileset]")
{
  const QImage image{"terrain.png"};
  tileset sheet{image, 32, 32};

  CHECK(sheet.num_tiles() == 1024);
}

TEST_CASE("tileset::last_id", "[tileset]")
{
  tileset sheet{"terrain.png", 32, 32};

  CHECK(sheet.last_id() == tile_id{1025});
  CHECK(sheet.last_id() - sheet.first_id() == tile_id{sheet.num_tiles()});

  sheet.set_first_id(tile_id{43});
  CHECK(sheet.last_id() - sheet.first_id() == tile_id{sheet.num_tiles()});
}

TEST_CASE("tileset::contains", "[tileset]")
{
  tileset sheet{"terrain.png", 32, 32};

  CHECK(sheet.contains(sheet.first_id()));
  CHECK(sheet.contains(sheet.last_id()));
  CHECK(!sheet.contains(sheet.first_id() - tile_id{1}));
  CHECK(!sheet.contains(sheet.last_id() + tile_id{1}));

  const tile_id id{82};
  sheet.set_first_id(id);
  CHECK(sheet.contains(id));
  CHECK(sheet.contains(id + tile_id{5}));
  CHECK(sheet.contains(sheet.last_id()));
}

TEST_CASE("tileset::tile_at", "[tileset]")
{
  SECTION("Outside of the tile sheet area")
  {
    tileset sheet{"terrain.png", 32, 32};

    CHECK(sheet.tile_at(-1, -1) == empty);
    CHECK(sheet.tile_at(sheet.width() + 1, 0) == empty);
    CHECK(sheet.tile_at(0, sheet.height() + 1) == empty);
    CHECK(sheet.tile_at(sheet.width() + 1, sheet.height() + 1) == empty);
  }

  SECTION("Without changed first ID")
  {
    tileset sheet{"terrain.png", 32, 32};

    const auto row = 7;
    const auto col = 5;
    const auto x = col * sheet.get_tile_width() + 13;
    const auto y = row * sheet.get_tile_height() + 24;

    const tile_id index{row * sheet.cols() + col};
    CHECK(sheet.tile_at(x, y) == sheet.first_id() + index);
  }

  SECTION("With changed first ID")
  {
    tileset sheet{"terrain.png", 32, 32};

    const tile_id first{38};
    sheet.set_first_id(first);

    const auto row = 9;
    const auto col = 4;
    const auto x = col * sheet.get_tile_width();
    const auto y = row * sheet.get_tile_height();

    const tile_id index{row * sheet.cols() + col};
    CHECK(sheet.tile_at(x, y) == sheet.first_id() + index);
  }
}

TEST_CASE("tileset::tile_width", "[tileset]")
{
  SECTION("Good size")
  {
    tileset sheet{"terrain.png", 32, 15};
    CHECK(sheet.get_tile_width() == 32);
  }

  SECTION("Clamping bad size")
  {
    tileset sheet{"terrain.png", 0, 15};
    CHECK(sheet.get_tile_width() == 1);
  }
}

TEST_CASE("tileset::tile_height", "[tileset]")
{
  SECTION("Good size")
  {
    tileset sheet{"terrain.png", 15, 32};
    CHECK(sheet.get_tile_height() == 32);
  }

  SECTION("Clamping bad size")
  {
    tileset sheet{"terrain.png", 32, 0};
    CHECK(sheet.get_tile_height() == 1);
  }
}
