#pragma once

#include <QDialog>    // QDialog
#include <QFileInfo>  // QFileInfo

#include "smart_pointers.hpp"

namespace Ui {
class open_map_error_dialog;
}

namespace tactile::gui {

class open_map_error_dialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit open_map_error_dialog(QWidget* parent = nullptr);

  ~open_map_error_dialog() noexcept override;

  void set_file(const QFileInfo& path);

  void set_error_message(const QString& message);

 private:
  unique<Ui::open_map_error_dialog> m_ui;
};

}  // namespace tactile::gui
