#pragma once

#include <QGraphicsView>
#include <QRect>

#include "core_model.hpp"
#include "tilemap_scene.hpp"

namespace tactile::gui {

/**
 * @class tilemap_view
 *
 * @brief Represents the UI component that holds a tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile tilemap_view.hpp
 */
class tilemap_view final : public QGraphicsView
{
  Q_OBJECT

 public:
  explicit tilemap_view(not_null<model::tilemap*> map,
                        int id,
                        QWidget* parent = nullptr);

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  [[nodiscard]] auto id() const noexcept -> int;

  void force_redraw();

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  //  bool event(QEvent* event) override;

 private:
  QPoint m_lastMousePos{};
  QPointF m_lastMouseScenePos{};

  [[nodiscard]] auto get_scene() const noexcept -> const tilemap_scene*;
};

}  // namespace tactile::gui
