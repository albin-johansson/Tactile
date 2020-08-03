#pragma once

#include <QTabWidget>
#include <optional>
#include <unordered_map>

#include "tactile_fwd.hpp"

namespace tactile::ui {

class tilemap_tab_widget final : public QTabWidget {
  Q_OBJECT

 public:
  explicit tilemap_tab_widget(QWidget* parent = nullptr);

  ~tilemap_tab_widget() noexcept override;

  [[nodiscard]] auto add_tile_map_tab(const QString& title) noexcept -> int;

  void remove_tile_map_tab(int id) noexcept;

  void center_viewport(int mapWidth, int mapHeight) noexcept;

  void move_viewport(int dx, int dy) noexcept;

  [[nodiscard]] auto active_tab_id() const noexcept -> std::optional<int>;

  [[nodiscard]] auto tab_id(int index) const noexcept -> std::optional<int>;

 signals:
  void s_redraw(QPainter& painter);

  void s_remove_tab(int id);

 private:
  [[nodiscard]] auto get_pane(int index) const noexcept -> editor_tab*;
};

}  // namespace tactile::ui
