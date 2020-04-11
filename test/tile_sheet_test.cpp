#include "tile_sheet.h"

#include <QImage>
#include <catch.hpp>
#include <memory>

#include "tactile_types.h"

using namespace tactile;

TEST_CASE("TileSheet(const SharedPtr<QImage>&, int, int)", "[TileSheet]")
{
  SECTION("Null image")
  {
    Shared<QImage> image = nullptr;
    CHECK_THROWS_AS(TileSheet(nullptr, 10, 10), BadArg);
  }

  SECTION("Zero size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    CHECK_NOTHROW(TileSheet{image, 0, 0});
  }

  SECTION("Good args")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    CHECK_NOTHROW(TileSheet{image, 32, 32});
  }
}

TEST_CASE("TileSheet::unique", "[TileSheet]")
{
  CHECK_THROWS_AS(TileSheet::unique(nullptr, 32, 32), BadArg);

  auto image = std::make_shared<QImage>("terrain.png");
  CHECK(TileSheet::unique(image, 32, 32));
}

TEST_CASE("TileSheet::shared", "[TileSheet]")
{
  CHECK_THROWS_AS(TileSheet::shared(nullptr, 32, 32), BadArg);

  auto image = std::make_shared<QImage>("terrain.png");
  CHECK(TileSheet::shared(image, 32, 32));
}

TEST_CASE("TileSheet::select", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  const auto tileWidth = 32;
  const auto tileHeight = 32;
  TileSheet sheet{image, tileWidth, tileHeight};

  sheet.select(0, 0);
  CHECK(sheet.selection().size() == 1);

  sheet.select(0, 0);
  CHECK(sheet.selection().size() == 1);

  sheet.select(3 * tileWidth, 2 * tileHeight);
  CHECK(sheet.selection().size() == 2);

  sheet.select(-1, -1);
  CHECK(sheet.selection().size() == 2);

  sheet.clear_selection();
  CHECK(sheet.selection().empty());
}

TEST_CASE("TileSheet::set_first_id", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  TileSheet sheet{image, 32, 32};

  CHECK(sheet.first_id() == 1);

  const auto id = 84;
  sheet.set_first_id(id);

  CHECK(sheet.first_id() == id);

  sheet.set_first_id(0);  // invalid
  CHECK(sheet.first_id() == id);

  sheet.set_first_id(-1);  // invalid
  CHECK(sheet.first_id() == id);
}

TEST_CASE("TileSheet::width", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  TileSheet sheet{image, 32, 32};

  CHECK(sheet.width() == image->width());
}

TEST_CASE("TileSheet::height", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("outside.png");
  TileSheet sheet{image, 32, 32};

  CHECK(sheet.height() == image->height());
}

TEST_CASE("TileSheet::tiles", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("outside.png");
  TileSheet sheet{image, 32, 32};

  CHECK(sheet.tiles() == 1024);
}

TEST_CASE("TileSheet::last_id", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("outside.png");
  TileSheet sheet{image, 32, 32};

  CHECK(sheet.last_id() == 1025);
  CHECK(sheet.last_id() - sheet.first_id() == sheet.tiles());

  sheet.set_first_id(43);
  CHECK(sheet.last_id() - sheet.first_id() == sheet.tiles());
}

TEST_CASE("TileSheet::contains", "[TileSheet]")
{
  auto image = std::make_shared<QImage>("terrain.png");
  TileSheet sheet{image, 32, 32};

  CHECK(sheet.contains(sheet.first_id()));
  CHECK(sheet.contains(sheet.last_id()));
  CHECK(!sheet.contains(sheet.first_id() - 1));
  CHECK(!sheet.contains(sheet.last_id() + 1));

  const auto id = 82;
  sheet.set_first_id(id);
  CHECK(sheet.contains(id));
  CHECK(sheet.contains(id + 5));
  CHECK(sheet.contains(sheet.last_id()));
}

TEST_CASE("TileSheet::tile_at", "[TileSheet]")
{
  SECTION("Outside of the tile sheet area")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32, 32};

    CHECK(sheet.tile_at(-1, -1) == empty);
    CHECK(sheet.tile_at(sheet.width() + 1, 0) == empty);
    CHECK(sheet.tile_at(0, sheet.height() + 1) == empty);
    CHECK(sheet.tile_at(sheet.width() + 1, sheet.height() + 1) == empty);
  }

  SECTION("Without changed first ID")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32, 32};

    const auto row = 7;
    const auto col = 5;
    const auto x = col * sheet.tile_width() + 13;
    const auto y = row * sheet.tile_height() + 24;

    const auto index = row * sheet.cols() + col;
    CHECK(sheet.tile_at(x, y) == sheet.first_id() + index);
  }

  SECTION("With changed first ID")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32, 32};

    const auto first = 38;
    sheet.set_first_id(first);

    const auto row = 9;
    const auto col = 4;
    const auto x = col * sheet.tile_width();
    const auto y = row * sheet.tile_height();

    const auto index = row * sheet.cols() + col;
    CHECK(sheet.tile_at(x, y) == sheet.first_id() + index);
  }
}

TEST_CASE("TileSheet::tile_width", "[TileSheet]")
{
  SECTION("Good size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32, 15};
    CHECK(sheet.tile_width() == 32);
  }

  SECTION("Clamping bad size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 0, 15};
    CHECK(sheet.tile_width() == 1);
  }
}

TEST_CASE("TileSheet::tile_height", "[TileSheet]")
{
  SECTION("Good size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 15, 32};
    CHECK(sheet.tile_height() == 32);
  }

  SECTION("Clamping bad size")
  {
    auto image = std::make_shared<QImage>("terrain.png");
    TileSheet sheet{image, 32, 0};
    CHECK(sheet.tile_height() == 1);
  }
}

TEST_CASE("TileSheet::selection", "[TileSheet]")
{
  // TODO
}