#include "flood_fill.h"

#include <queue>

#include "map_position.h"

namespace tactile {

void flood_fill(TileLayer& layer,
                const MapPosition& origin,
                TileID target,
                TileID replacement) noexcept
{
  const bool returnImmediately = !layer.in_bounds(origin) ||
                                 (target == replacement) ||
                                 (layer.tile_at(origin) != target);
  if (returnImmediately) {
    return;
  }

  layer.set_tile(origin, replacement);

  std::queue<MapPosition> queue;
  queue.push(origin);

  const auto update = [&layer, &queue, target, replacement](
                          const MapPosition& position) noexcept {
    const auto tile = layer.tile_at(position);
    if (tile && tile == target) {
      layer.set_tile(position, replacement);
      queue.push(position);
    }
  };

  while (!queue.empty()) {
    const auto pos = queue.front();
    queue.pop();

    update(pos.west());
    update(pos.east());
    update(pos.south());
    update(pos.north());
  }
}

}  // namespace tactile