#include "properties_dock.hpp"

#include "properties_widget.hpp"

namespace tactile::gui {

properties_dock::properties_dock(QWidget* parent)
    : dock_widget{parent},
      m_widget{new properties_widget{this}}
{
  setObjectName(QStringLiteral(u"properties_dock"));
  setWindowTitle(tr("Properties"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setWidget(m_widget);
}

}  // namespace tactile::gui
