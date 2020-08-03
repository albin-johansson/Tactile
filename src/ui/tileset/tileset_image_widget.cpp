#include "tileset_image_widget.hpp"

#include <QGridLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>

#include "tactile_error.hpp"
#include "tileset_image_label.hpp"

namespace tactile::ui {

tileset_image_widget::tileset_image_widget(const QImage& image,
                                           int tileWidth,
                                           int tileHeight,
                                           QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw tactile_error{"Can't create tileset image widget from null image!"};
  }

  m_layout = new QGridLayout{this};

  m_imageLabel = new tileset_image_label{image, tileWidth, tileHeight};
  m_rubberBand = new QRubberBand{QRubberBand::Rectangle, m_imageLabel};

  m_layout->addWidget(m_imageLabel);  // layout claims ownership of label

  setLayout(m_layout);
}

tileset_image_widget::~tileset_image_widget() noexcept = default;

void tileset_image_widget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  if (event->buttons() & Qt::MouseButton::LeftButton &&
      !m_rubberBand->isVisible()) {
    m_origin = event->pos();

    m_rubberBand->setGeometry(QRect{m_origin, QSize()});
    m_rubberBand->show();
  }
}

void tileset_image_widget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    m_rubberBand->setGeometry(QRect{m_origin, event->pos()}.normalized());
  }
}

void tileset_image_widget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);

  if (event->button() == Qt::MouseButton::LeftButton) {
    m_rubberBand->hide();
  }
  // TODO compute the selection
}

}  // namespace tactile::ui
