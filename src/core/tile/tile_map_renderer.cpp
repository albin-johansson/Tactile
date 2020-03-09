#include "tile_map_renderer.h"

#include <QPainter>

#include "tile_map.h"
#include "tile_size.h"

namespace tactile {

void TileMapRenderer::render(QPainter& painter, const TileMap& map) const
    noexcept
{
  const auto& viewport = painter.viewport();
  const auto tileSize = TileSize::get().size();

  const auto nRows = map.m_nRows;
  const auto nCols = map.m_nCols;

  const auto minCol = (viewport.x() > 0) ? 0 : (-viewport.x() / tileSize);
  const auto minRow = (viewport.y() > 0) ? 0 : (-viewport.y() / tileSize);

  const auto calcMaxCol = (viewport.width() - viewport.x()) / tileSize + 1;
  const auto calcMaxRow = (viewport.height() - viewport.y()) / tileSize + 1;
  const auto maxCol = (calcMaxCol > nCols) ? nCols : calcMaxCol;
  const auto maxRow = (calcMaxRow > nRows) ? nRows : calcMaxRow;

  QPen pen;
  pen.setColor(Qt::black);
  pen.setWidth(1);
  pen.setDashOffset(1);
  pen.setDashPattern({2, 3});

  painter.setPen(pen);

  for (auto& layer : map.m_layers) {
    // TODO...

    for (auto row = minRow; row < maxRow; ++row) {
      for (auto col = minCol; col < maxCol; ++col) {
        const auto x = col * tileSize;
        const auto y = row * tileSize;
        painter.fillRect(x, y, tileSize, tileSize, Qt::darkGray);
        painter.drawRect(x, y, tileSize, tileSize);
      }
    }
  }
}

}  // namespace tactile
