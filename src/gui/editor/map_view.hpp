#pragma once

#include <qgraphicsview.h>
#include <qrect.h>

#include "map_scene.hpp"
#include "model.hpp"

namespace tactile::gui {

/**
 * @class map_view
 *
 * @brief Represents the UI component that presents a map.
 *
 * @since 0.1.0
 *
 * @headerfile map_view.hpp
 */
class map_view final : public QGraphicsView
{
  Q_OBJECT

 public:
  explicit map_view(not_null<core::map*> map,
                    map_id id,
                    QWidget* parent = nullptr);

  void force_redraw();

  void center_map();

  void move_map(int dx, int dy);

  [[nodiscard]] auto id() const -> map_id;

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  QPoint m_lastMousePos{};

  [[nodiscard]] auto get_map_scene() -> map_scene*;

  [[nodiscard]] auto get_map_scene() const -> const map_scene*;
};

}  // namespace tactile::gui
