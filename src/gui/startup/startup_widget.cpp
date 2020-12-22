#include "startup_widget.hpp"

#include "init_ui.hpp"
#include "ui_startup_widget.h"

namespace tactile::gui {

startup_widget::startup_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{init_ui<Ui::startup_widget>(this)}
{}

startup_widget::~startup_widget() noexcept
{
  delete m_ui;
}

}  // namespace tactile::gui
