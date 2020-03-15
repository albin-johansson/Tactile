#include "render_pane.h"

#include <QApplication>
#include <QResizeEvent>
#include <QPainter>

#include "widget_size_policy.h"

namespace tactile {

RenderPane::RenderPane(QWidget* parent)
    : QWidget{parent},
      m_viewport{0, 0, width(), height()},
      m_lastMouseX{0},
      m_lastMouseY{0}
{
  set_size_policy(this, QSizePolicy::Policy::Expanding);
}

void RenderPane::center_viewport(int mapWidth, int mapHeight)
{
  const auto x = (m_viewport.width() - mapWidth) / 2;
  const auto y = (m_viewport.height() - mapHeight) / 2;

  m_viewport.moveTo(x, y);
}

void RenderPane::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);

  QPainter painter{this};
  painter.fillRect(0, 0, width(), height(), Qt::black);
  painter.setViewport(m_viewport);

  emit rp_req_redraw(painter);
}

void RenderPane::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);

  const auto& newSize = event->size();
  const auto newWidth = newSize.width();
  const auto newHeight = newSize.height();

  m_viewport.setWidth(newWidth);
  m_viewport.setHeight(newHeight);

  update();
}

void RenderPane::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);
  if (event->buttons() & Qt::MouseButton::MidButton) {
    m_lastMouseX = event->x();
    m_lastMouseY = event->y();
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }
}

void RenderPane::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
  QApplication::restoreOverrideCursor();
}

void RenderPane::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);
  if (event->buttons() & Qt::MouseButton::MidButton) {
    m_viewport.translate(event->x() - m_lastMouseX, event->y() - m_lastMouseY);
    m_lastMouseX = event->x();
    m_lastMouseY = event->y();
    update();
  }
}

}  // namespace tactile
