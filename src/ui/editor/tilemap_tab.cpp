#include "tilemap_tab.hpp"

#include <algorithm>

#include "tilemap_scene.hpp"

namespace tactile::ui {

tilemap_tab::tilemap_tab(QWidget* parent) : QTabWidget{parent}
{
  setTabsClosable(true);
  connect(this, &QTabWidget::tabCloseRequested, this, [this](int index) {
    emit request_remove_tab(get_pane(index)->id());
    removeTab(index);
  });
}

tilemap_tab::~tilemap_tab() noexcept = default;

auto tilemap_tab::add_tile_map_tab(const QString& title) noexcept -> int
{
  static int id = 0;  // serial ID

  auto newTitle = title;
  newTitle.append(QString::number(id));

  auto* pane = new tilemap_scene{id++};
  addTab(pane, newTitle);

  connect(
      pane, &tilemap_scene::s_redraw, this, &tilemap_tab::request_redraw);

  return pane->id();
}

void tilemap_tab::remove_tile_map_tab(int id) noexcept
{
  const auto amount = count();
  for (int i = 0; i < amount; ++i) {
    if (auto* pane = get_pane(i); pane && pane->id() == id) {
      removeTab(i);
    }
  }
}

void tilemap_tab::center_viewport(int mapWidth, int mapHeight) noexcept
{
  if (count() > 0) {
    if (auto* pane = get_pane(currentIndex()); pane) {
      pane->center_viewport(mapWidth, mapHeight);
    }
  }
}

void tilemap_tab::move_viewport(int dx, int dy) noexcept
{
  if (count() > 0) {
    if (auto* pane = get_pane(currentIndex()); pane) {
      pane->move_viewport(dx, dy);
    }
  }
}

auto tilemap_tab::get_pane(int index) const noexcept -> tilemap_scene*
{
  return qobject_cast<tilemap_scene*>(widget(index));
}

auto tilemap_tab::active_tab_id() const noexcept -> std::optional<int>
{
  return tab_id(currentIndex());
}

auto tilemap_tab::tab_id(int index) const noexcept -> std::optional<int>
{
  if (auto* pane = get_pane(index); pane) {
    return pane->id();
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile::ui
