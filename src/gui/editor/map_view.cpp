#include "map_view.hpp"

#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QScrollBar>

namespace tactile::gui {

map_view::map_view(not_null<model::tilemap*> map,
                           map_id id,
                           QWidget* parent)
    : QGraphicsView{parent}
{
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  setScene(new map_scene{map, id, this});
}

void map_view::move_viewport(int dx, int dy) noexcept
{
  const auto old = scene()->sceneRect();
  scene()->setSceneRect(old.x() + dx, old.y() + dy, old.width(), old.height());
}

void map_view::center_viewport(int mapWidth, int mapHeight) noexcept
{
  // TODO
  //  m_scene->center_viewport(mapWidth, mapHeight);
}

void map_view::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);
}

void map_view::mouseMoveEvent(QMouseEvent* event)
{
  QGraphicsView::mouseMoveEvent(event);

  m_lastMousePos = event->globalPos();
  m_lastMouseScenePos = mapToScene(viewport()->mapFromGlobal(m_lastMousePos));

  //    if (event->buttons() & Qt::MouseButton::MidButton) {
  //      const auto pos = event->pos();
  //
  //      const int x = pos.x();
  //      const int y = pos.y();
  //
  //      translate(x - m_lastMouseX, y - m_lastMouseY);
  //
  //      m_lastMouseX = x;
  //      m_lastMouseY = y;
  //      update();
  //    }
}

void map_view::mouseReleaseEvent(QMouseEvent* event)
{
  QGraphicsView::mouseReleaseEvent(event);
}

auto map_view::id() const noexcept -> map_id
{
  return get_scene()->id();
}

void map_view::force_redraw()
{
  scene()->update(0, 0, 1'000, 1'000);  // FIXME
}

auto map_view::get_scene() const noexcept -> const map_scene*
{
  return qobject_cast<const map_scene*>(scene());
}

// bool tilemap_view::event(QEvent* event)
//{
//  if (const auto type = event->type();
//      type == QEvent::TouchBegin || type == QEvent::TouchUpdate ||
//      type == QEvent::TouchEnd || type == QEvent::TouchCancel) {
//    qInfo("Touch!");
//    return QGraphicsView::event(event);
//  } else {
//    return QGraphicsView::event(event);
//  }
//}

}  // namespace tactile::gui