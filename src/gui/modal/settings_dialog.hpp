#pragma once

#include <QDialog>
#include <QWidget>
#include <QString>

namespace Ui {

class SettingsUI;

}

namespace tactile::gui {

class settings_dialog final : public QDialog {
  Q_OBJECT

 public:
  explicit settings_dialog(QWidget* parent = nullptr);

  ~settings_dialog() noexcept override;

 signals:
  void reload_stylesheet();

 private:
  Ui::SettingsUI* m_ui;
  QString m_initialTheme;

 private slots:
  void handle_accept();
};

}  // namespace tactile::gui
