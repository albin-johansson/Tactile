#pragma once

#include <QDialog>    // QDialog
#include <QFileInfo>  // QFileInfo

#include "smart_pointers.hpp"
#include "tactile_declare_ui.hpp"

TACTILE_DECLARE_UI(OpenMapErrorDialog)

namespace tactile {

class OpenMapErrorDialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit OpenMapErrorDialog(QWidget* parent = nullptr);

  ~OpenMapErrorDialog() noexcept override;

  void SetFile(const QFileInfo& path);

  void SetErrorMessage(const QString& message);

 private:
  Unique<Ui::OpenMapErrorDialog> mUi;
};

}  // namespace tactile
