#pragma once
#include <QRect>
#include <QWidget>

class QPainter;

namespace tactile::ui {

class editor_tab final : public QWidget {
  Q_OBJECT

 public:
  explicit editor_tab(int id, QWidget* parent = nullptr);

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  [[nodiscard]] auto id() const noexcept -> int;

 signals:
  void s_redraw(QPainter& painter);

 protected:
  void paintEvent(QPaintEvent* event) override;

  void resizeEvent(QResizeEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QRect m_viewport;
  int m_id;
  int m_lastMouseX{};
  int m_lastMouseY{};
};

}  // namespace tactile::ui
