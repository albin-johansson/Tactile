#pragma once

#include <qlayout.h>
#include <qscrollarea.h>
#include <qwidget.h>

#include "tactile_fwd.hpp"

namespace tactile::gui {

class tileset_tab final : public QWidget
{
 public:
  explicit tileset_tab(const QImage& image,
                       int tileWidth,
                       int tileHeight,
                       QWidget* parent = nullptr);

  ~tileset_tab() noexcept override;

 private:
  QScrollArea* m_scrollArea;
  tileset_image_widget* m_imageWidget;
  QLayout* m_layout;
};

}  // namespace tactile::gui
