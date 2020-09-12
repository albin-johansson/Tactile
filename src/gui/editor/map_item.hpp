#pragma once

#include <qgraphicsitem.h>

#include <optional>

#include "fwd.hpp"
#include "model.hpp"
#include "tileset.hpp"

namespace tactile::gui {

/**
 * @class map_item
 *
 * @brief Responsible for the visual representation of maps.
 *
 * @since 0.1.0
 *
 * @headerfile map_item.hpp
 */
class map_item final : public QGraphicsItem
{
 public:
  /**
   * @brief Creates a map item.
   *
   * @param map the associated map, cannot be null.
   * @param tilesets the associated tileset model, cannot be null.
   * @param parent the parent item.
   *
   * @since 0.1.0
   */
  explicit map_item(not_null<core::map*> map,
                    not_null<core::tileset_manager*> tilesets,
                    QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  void disable_stamp_preview();

  void enable_stamp_preview(const core::position& position);

  [[nodiscard]] auto boundingRect() const -> QRectF override;

 private:
  core::map* m_map{};
  core::tileset_manager* m_tilesets{};
  std::optional<core::position> m_mousePosition;

  void draw_layer(QPainter& painter,
                  const core::layer& layer,
                  const QRectF& exposed,
                  int tileSize);

  void draw_tile(QPainter& painter, tile_id tile, int x, int y, int tileSize);

  void draw_preview(QPainter& painter, int tileSize);

  void draw_preview_multiple_tiles(QPainter& painter,
                                   const core::position& mousePosition,
                                   const core::tileset::selection& selection,
                                   int tileSize);
};

}  // namespace tactile::gui
