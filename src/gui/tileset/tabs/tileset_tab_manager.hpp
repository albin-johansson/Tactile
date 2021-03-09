#pragma once

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "tileset_id.hpp"
#include "vector_map.hpp"

TACTILE_FORWARD_DECLARE(tactile::gui, tileset_tab)

namespace tactile::gui {

class tileset_tab_manager final
{
 public:
  void add(tileset_id id, tileset_tab* tab);

  void remove(tileset_id id);

  void clear() noexcept;

  void set_cached_index(int index);

  [[nodiscard]] auto at(tileset_id id) const -> const tileset_tab*;

  [[nodiscard]] auto index_of(tileset_id id) const -> int;

  [[nodiscard]] auto contains(tileset_id id) const -> bool;

  [[nodiscard]] auto is_empty() const -> bool;

  [[nodiscard]] auto cached_index() const -> maybe<int>
  {
    return m_cachedIndex;
  }

  [[nodiscard]] auto begin() noexcept
  {
    return m_tabs.begin();
  }

  [[nodiscard]] auto begin() const noexcept
  {
    return m_tabs.begin();
  }

  [[nodiscard]] auto end() noexcept
  {
    return m_tabs.end();
  }

  [[nodiscard]] auto end() const noexcept
  {
    return m_tabs.end();
  }

 private:
  vector_map<tileset_id, tileset_tab*> m_tabs;
  maybe<int> m_cachedIndex;
};

}  // namespace tactile::gui
