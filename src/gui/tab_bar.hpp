#pragma once

#include <QLineEdit>
#include <QTabBar>
#include <optional>  // optional

namespace tactile::gui {

class tab_bar final : public QTabBar
{
  Q_OBJECT

 public:
  explicit tab_bar(QWidget* parent = nullptr);

  void edit_tab(int index);

 signals:
  void edited_tab(int index);

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 private:
  QLineEdit* m_edit{};
  std::optional<int> m_renameIndex;
};

}  // namespace tactile::gui
