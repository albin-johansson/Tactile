#include "open_map_error_dialog.hpp"

#include "ui_open_map_error_dialog.h"

namespace tactile::gui {

open_map_error_dialog::open_map_error_dialog(QWidget* parent)
    : QDialog{parent},
      m_ui{new Ui::open_map_error_dialog{}}
{
  m_ui->setupUi(this);
}

open_map_error_dialog::~open_map_error_dialog() noexcept
{
  delete m_ui;
}

void open_map_error_dialog::set_file(const QFileInfo& path)
{
  m_ui->filePathLabel->setText(path.fileName());
}

void open_map_error_dialog::set_error_message(const QString& message)
{
  m_ui->errorMessageLabel->setText(message);
}

}  // namespace tactile::gui
