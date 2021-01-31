#include "layer_dock.hpp"

#include "layer_widget.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

layer_dock::layer_dock(QWidget* parent)
    : dock_widget{parent}
    , m_widget{new layer_widget{this}}
{
  setObjectName(TACTILE_QSTRING(u"layer_dock"));
  setWindowTitle(tr("Layers"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(m_widget);
}

void layer_dock::switched_map(not_null<core::map_document*> document)
{
  m_widget->selected_map(document);
}

}  // namespace tactile::gui
