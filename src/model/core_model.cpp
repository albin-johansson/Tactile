#include "core_model.hpp"

#include <QImage>

#include "render_tilemap.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "tileset_model.hpp"

namespace tactile::model {

void core_model::undo()
{
  if (auto* map = current_map(); map) {
    map->undo();
    emit redraw_requested();
  }
}

void core_model::redo()
{
  if (auto* map = current_map(); map) {
    map->redo();
    emit redraw_requested();
  }
}

void core_model::resize_map(int nRows, int nCols)
{
  if (auto* map = current_map(); map) {
    map->resize(nRows, nCols);
    emit redraw_requested();
  }
}

void core_model::add_row() noexcept
{
  if (auto* map = current_map(); map) {
    map->add_row();
    emit redraw_requested();
  }
}

void core_model::add_col() noexcept
{
  if (auto* map = current_map(); map) {
    map->add_column();
    emit redraw_requested();
  }
}

void core_model::remove_row() noexcept
{
  if (auto* map = current_map(); map) {
    map->remove_row();
    emit redraw_requested();
  }
}

void core_model::remove_col() noexcept
{
  if (auto* map = current_map(); map) {
    map->remove_column();
    emit redraw_requested();
  }
}

auto core_model::add_map() -> int
{
  const auto id = m_nextMapID;
  Q_ASSERT(!m_maps.count(id));

  auto* map = new map_model{this};

  auto bind = [map, this](auto&& signal, auto&& slot) {
    connect(map, signal, this, slot);
  };

  bind(&map_model::undo_state_updated, &core_model::undo_state_updated);
  bind(&map_model::redo_state_updated, &core_model::redo_state_updated);
  bind(&map_model::undo_text_updated, &core_model::undo_text_updated);
  bind(&map_model::redo_text_updated, &core_model::redo_text_updated);

  m_maps.emplace(id, map);
  m_currentMapID = id;

  ++m_nextMapID;

  return id;
}

void core_model::handle_close_map(map_id id) noexcept
{
  Q_ASSERT(m_maps.count(id));

  auto* map = m_maps.at(id);
  map->disconnect();

  m_maps.erase(id);

  if (m_currentMapID == id) {
    m_currentMapID = std::nullopt;
  }

  if (!m_maps.empty()) {
    const auto begin = m_maps.begin();
    m_currentMapID = begin->first;
  }
}

auto core_model::add_tileset(const QImage& image,
                             int tileWidth,
                             int tileHeight) noexcept -> std::optional<int>
{
  if (!image.isNull()) {
    return m_tilesets.emplace(image, tileWidth, tileHeight);
  } else {
    return std::nullopt;
  }
}

auto core_model::rows() const noexcept -> std::optional<int>
{
  if (auto* map = current_map(); map) {
    return map->rows();
  } else {
    return std::nullopt;
  }
}

auto core_model::cols() const noexcept -> std::optional<int>
{
  if (auto* map = current_map(); map) {
    return map->columns();
  } else {
    return std::nullopt;
  }
}

auto core_model::map_width() const noexcept -> std::optional<int>
{
  if (auto* map = current_map(); map) {
    return map->width();
  } else {
    return std::nullopt;
  }
}

auto core_model::map_height() const noexcept -> std::optional<int>
{
  if (auto* map = current_map(); map) {
    return map->height();
  } else {
    return std::nullopt;
  }
}

auto core_model::tile_size() const noexcept -> std::optional<int>
{
  if (auto* map = current_map(); map) {
    return map->current_tile_size();
  } else {
    return std::nullopt;
  }
}

void core_model::select_layer(int index) noexcept
{
  if (auto* map = current_map(); map) {
    map->select_layer(index);
    emit redraw_requested();
  }
}

void core_model::select_map(int id) noexcept
{
  Q_ASSERT(m_maps.count(id));

  if (m_currentMapID != id) {
    m_currentMapID = id;

    auto* map = current_map();
    Q_ASSERT(map);

    emit undo_state_updated(map->can_undo());
    emit redo_state_updated(map->can_redo());
    emit undo_text_updated(map->undo_text());
    emit redo_text_updated(map->redo_text());

    emit switched_map(id);

    // TODO need to signal window to switch active tab
  }
}

auto core_model::has_active_map() const noexcept -> bool
{
  return m_currentMapID.has_value();
}

void core_model::handle_increase_tile_size() noexcept
{
  if (auto* map = current_map(); map) {
    map->increase_tile_size();
    emit redraw_requested();
  }
}

void core_model::handle_decrease_tile_size() noexcept
{
  if (auto* map = current_map(); map) {
    map->decrease_tile_size();
    emit redraw_requested();
  }
}

void core_model::handle_reset_tile_size() noexcept
{
  if (auto* map = current_map(); map) {
    map->reset_tile_size();
    emit redraw_requested();
  }
}

void core_model::handle_draw(QPainter& painter, const QRectF& exposed)
{
  if (auto* map = current_map(); map) {
    map->draw(painter, exposed);
  }
}

auto core_model::current_map() -> map_model*
{
  return m_currentMapID ? m_maps.at(*m_currentMapID) : nullptr;
}

auto core_model::current_map() const -> const map_model*
{
  return m_currentMapID ? m_maps.at(*m_currentMapID) : nullptr;
}

}  // namespace tactile::model