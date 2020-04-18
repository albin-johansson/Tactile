#include "tile_sheet_image_widget.h"

#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>

#include "tactile_types.h"

namespace tactile {

class ImageLabel final : public QLabel {
 public:
  explicit ImageLabel(const QImage& image, QWidget* parent = nullptr)
      : QLabel{parent}
  {
    if (image.isNull()) {
      throw BadArg{"Can't create tile sheet image widget from null image!"};
    }
    setPixmap(QPixmap::fromImage(image));
  }

  ~ImageLabel() {}

 protected:
  void paintEvent(QPaintEvent* event) override
  {
    QLabel::paintEvent(event);

    QPainter painter{this};

    const auto w = width();
    const auto h = height();

    const auto nRows = h / 32;
    const auto nCols = w / 32;

    for (auto r = 0; r < nRows; ++r) {
      for (auto c = 0; c < nCols; ++c) {
        const auto x = c * 32;  // FIXME use tileset size
        const auto y = r * 32;
        painter.drawLine(x, 0, x, w);
        painter.drawLine(0, y, h, y);
      }
    }

    {
      // renders the bottom and right-hand end lines
      const auto maxX = w - 1;
      const auto maxY = h - 1;
      painter.drawLine(0, maxY, w, maxY);
      painter.drawLine(maxX, 0, maxX, h);
    }
  }
};

TileSheetImageWidget::TileSheetImageWidget(const QImage& image, QWidget* parent)
    : QWidget{parent}
{
  if (image.isNull()) {
    throw BadArg{"Can't create tile sheet image widget from null image!"};
  }

  m_imageLabel = std::make_unique<ImageLabel>(image);

  m_layout = std::make_unique<QGridLayout>();
  m_layout->addWidget(m_imageLabel.get());

  setLayout(m_layout.get());

  m_rubberBand =
      std::make_unique<QRubberBand>(QRubberBand::Rectangle, m_imageLabel.get());
}

TileSheetImageWidget::~TileSheetImageWidget() noexcept = default;

void TileSheetImageWidget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);
  m_origin = event->pos();

  m_rubberBand->setGeometry(QRect{m_origin, QSize()});
  m_rubberBand->show();
}

void TileSheetImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);
  m_rubberBand->hide();

  // TODO compute the selection
}

void TileSheetImageWidget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);
  m_rubberBand->setGeometry(QRect{m_origin, event->pos()}.normalized());
}

}  // namespace tactile
