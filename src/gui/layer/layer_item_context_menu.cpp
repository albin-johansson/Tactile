#include "layer_item_context_menu.hpp"

#include "icons.hpp"

namespace tactile::gui {

layer_item_context_menu::layer_item_context_menu(QWidget* parent)
    : QMenu{parent}
    , m_visibility{addAction(icons::visible(), tr("Toggle visibility"))}
    , m_sep0{addSeparator()}
    , m_moveUp{addAction(icons::move_up(), tr("Move layer up"))}
    , m_moveDown{addAction(icons::move_down(), tr("Move layer down"))}
    , m_sep1{addSeparator()}
    , m_duplicate{addAction(icons::duplicate(), tr("Duplicate layer"))}
    , m_remove{addAction(icons::remove(), tr("Remove layer"))}
{
  // clang-format off
  connect(m_visibility, &QAction::triggered, this, &layer_item_context_menu::toggle_visibility);
  connect(m_moveUp, &QAction::triggered, this, &layer_item_context_menu::move_layer_up);
  connect(m_moveDown, &QAction::triggered, this, &layer_item_context_menu::move_layer_down);
  connect(m_duplicate, &QAction::triggered, this, &layer_item_context_menu::duplicate_layer);
  connect(m_remove, &QAction::triggered, this, &layer_item_context_menu::remove_layer);
  // clang-format on
}

void layer_item_context_menu::set_visibility_enabled(const bool enabled)
{
  m_visibility->setEnabled(enabled);
}

void layer_item_context_menu::set_remove_enabled(const bool enabled)
{
  m_remove->setEnabled(enabled);
}

void layer_item_context_menu::set_move_up_enabled(const bool enabled)
{
  m_moveUp->setEnabled(enabled);
}

void layer_item_context_menu::set_move_down_enabled(const bool enabled)
{
  m_moveDown->setEnabled(enabled);
}

}  // namespace tactile::gui
