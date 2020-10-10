#include "tileset_manager.hpp"

#include <catch.hpp>
#include <memory>

#include "tileset.hpp"
#include "types.hpp"

using namespace tactile;
using namespace tactile::core;

namespace {

[[nodiscard]] auto make_tileset(tileset_manager& manager)
{
  return std::make_shared<tileset>(
      manager.next_global_tile_id(), "terrain.png", 32_tw, 32_th);
}

}  // namespace

TEST_CASE("tileset_manager::emplace", "[tileset_manager]")
{
  tileset_manager manager;

  CHECK(manager.sheets() == 0);
  CHECK(!manager.has_active_tileset());

  const auto fst = manager.next_tileset_id();
  manager.add(fst, make_tileset(manager));

  const auto snd = manager.next_tileset_id();
  manager.add(snd, make_tileset(manager));

  CHECK(snd > fst);
  CHECK(manager.sheets() == 2);
}

TEST_CASE("tileset_manager::remove", "[tileset_manager]")
{
  tileset_manager manager;
  CHECK_NOTHROW(manager.remove(tileset_id{0}));

  const auto id = 0_ts;
  manager.add(id, std::make_shared<tileset>(0_t, "terrain.png", 32_tw, 32_th));

  CHECK(manager.sheets() == 1);

  manager.remove(id + tileset_id{5});
  CHECK(manager.sheets() == 1);

  manager.remove(id);
  CHECK(manager.sheets() == 0);
}

TEST_CASE("tileset_manager::remove_all", "[tileset_manager]")
{
  tileset_manager manager;
  CHECK_NOTHROW(manager.remove_all());

  manager.add(manager.next_tileset_id(), make_tileset(manager));
  manager.add(manager.next_tileset_id(), make_tileset(manager));

  CHECK(manager.sheets() == 2);

  manager.remove_all();
  CHECK(manager.sheets() == 0);
}

TEST_CASE("tileset_manager::select", "[tileset_manager]")
{
  tileset_manager manager;

  CHECK(!manager.has_active_tileset());

  const auto id = manager.next_tileset_id();
  manager.add(id, make_tileset(manager));

  manager.select(id);
  CHECK(manager.has_active_tileset());

  manager.select(std::nullopt);
  CHECK(!manager.has_active_tileset());
}