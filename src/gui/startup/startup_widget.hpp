#pragma once

#include <QWidget>

namespace Ui {
class startup_widget;
}

namespace tactile::gui {

class startup_widget final : public QWidget
{
 public:
  explicit startup_widget(QWidget* parent = nullptr);

  ~startup_widget() noexcept override;

 private:
  Ui::startup_widget* m_ui;
};

}  // namespace tactile::gui
