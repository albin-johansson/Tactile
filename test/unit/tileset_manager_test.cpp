#include "tileset_manager.hpp"

#include <QImage>
#include <catch.hpp>

#include "tileset.hpp"

using namespace tactile;

namespace {

[[nodiscard]] Unique<Tileset> create_tile_sheet() noexcept
{
  auto image = std::make_shared<QImage>("terrain.png");
  return Tileset::unique(image, 32, 32);
}

}  // namespace

TEST_CASE("TilesetManager::add", "[TilesetManager]")
{
  TilesetManager manager;

  CHECK(!manager.add(nullptr).has_value());
  CHECK(manager.sheets() == 0);
  CHECK(!manager.has_active_tileset());

  const auto first = manager.add(create_tile_sheet());
  const auto second = manager.add(create_tile_sheet());
  CHECK(first.has_value());
  CHECK(second.has_value());

  CHECK(second > first);

  CHECK(manager.sheets() == 2);
}

TEST_CASE("TilesetManager::remove", "[TilesetManager]")
{
  TilesetManager manager;
  CHECK_NOTHROW(manager.remove(0));

  const auto id = manager.add(create_tile_sheet());
  CHECK(id.has_value());
  CHECK(manager.sheets() == 1);

  manager.remove(*id + 5);
  CHECK(manager.sheets() == 1);

  manager.remove(*id);
  CHECK(manager.sheets() == 0);
}

TEST_CASE("TilesetManager::remove_all", "[TilesetManager]")
{
  TilesetManager manager;
  CHECK_NOTHROW(manager.remove_all());

  const auto a = manager.add(create_tile_sheet());
  const auto b = manager.add(create_tile_sheet());

  CHECK(manager.sheets() == 2);

  manager.remove_all();
  CHECK(manager.sheets() == 0);
}

TEST_CASE("TilesetManager::select", "[TilesetManager]")
{
  TilesetManager manager;

  CHECK_NOTHROW(manager.select(9));
  CHECK(!manager.has_active_tileset());

  const auto id = manager.add(create_tile_sheet());
  manager.add(create_tile_sheet());  // NOLINT

  manager.select(id);
  CHECK(manager.has_active_tileset());

  manager.select(nothing);
  CHECK(!manager.has_active_tileset());
}