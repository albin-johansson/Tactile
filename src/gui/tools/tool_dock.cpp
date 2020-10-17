#include "tool_dock.hpp"

#include <QLayout>

namespace tactile::gui {

tool_dock::tool_dock(QWidget* parent)
    : QDockWidget{parent},
      m_widget{new tool_widget{this}}
{
  setObjectName(QStringLiteral(u"tool_dock"));
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  setWidget(m_widget);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  // clang-format off
  connect(m_widget, &tool_widget::stamp_enabled, this, &tool_dock::enable_stamp);
  connect(m_widget, &tool_widget::bucket_enabled, this, &tool_dock::enable_bucket);
  connect(m_widget, &tool_widget::eraser_enabled, this, &tool_dock::enable_eraser);
  // clang-format on
}

}  // namespace tactile::gui
