#pragma once

#include <QFileInfo>  // QFileInfo
#include <QWidget>    // QWidget

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(Ui, file_value_widget)

namespace tactile {

class FileValueWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit FileValueWidget(QWidget* parent = nullptr);

  ~FileValueWidget() noexcept override;

  void EnterActiveMode();

  void EnterIdleMode();

  void ResetPath();

  void SetPath(const QString& path);

  [[nodiscard]] auto CurrentPath() const -> QString;

 signals:
  void S_SpawnDialog();

 private:
  unique<Ui::file_value_widget> mUi;
};

}  // namespace tactile
