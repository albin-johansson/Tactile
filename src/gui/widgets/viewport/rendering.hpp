#pragma once

#include "aliases/tile_id.hpp"
#include "imgui.h"

namespace Tactile {

class Tileset;
class TilesetManager;

[[nodiscard]] auto GetTileSizeUV(const Tileset& tileset) -> ImVec2;

void RenderTile(tile_id tile,
                const TilesetManager& tilesets,
                const ImVec2& screenPos,
                const ImVec2& gridSize);

}  // namespace Tactile
