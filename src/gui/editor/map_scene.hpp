#pragma once

#include <qgraphicsscene.h>
#include <qrect.h>
#include <qwidget.h>

#include "fwd.hpp"
#include "model.hpp"

class QPainter;

namespace tactile::gui {

class map_scene final : public QGraphicsScene
{
  Q_OBJECT

 public:
  explicit map_scene(not_null<core::map*> map,
                     map_id id,
                     QWidget* parent = nullptr);

  void move_map(int dx, int dy);

  void center_map();

  [[nodiscard]] auto map_position() const -> QPointF;

  [[nodiscard]] auto id() const noexcept -> map_id
  {
    return m_id;
  }

 protected:
  void drawBackground(QPainter* painter, const QRectF& rect) override;

  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

 private:
  map_item* m_item;
  map_id m_id;
  QPointF m_lastMouseScenePos{};
};

}  // namespace tactile::gui
