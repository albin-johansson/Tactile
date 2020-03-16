#include "mouse_tool_widget.h"

#include "ui_tool_button_widget.h"
#include "widget_size_policy.h"

namespace tactile {

MouseToolWidget::MouseToolWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::ToolButtonBarUI{}}
{
  m_ui->setupUi(this);
//  set_size_policy(this, QSizePolicy::Minimum);
//  setObjectName("");
}

MouseToolWidget::~MouseToolWidget() noexcept
{
  delete m_ui;
}

}  // namespace tactile
