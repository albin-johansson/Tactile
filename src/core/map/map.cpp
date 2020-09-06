#include "map.hpp"

#include <qdebug.h>

#include <cassert>

#include "algorithm.hpp"

namespace tactile::core {

map::map(int nRows, int nCols)
    : m_nRows{at_least(nRows, 1)}, m_nCols{at_least(nCols, 1)}
{
  m_layers.reserve(5);
  m_layers.emplace_back(nRows, nCols);
}

void map::set_tile(const position& pos, tile_id id)
{
  qDebug("map > setting tile (%i, %i) to %i",
         pos.get_row().get(),
         pos.get_col().get(),
         id.get());
  m_layers.at(m_activeLayer.get()).set_tile(pos, id);
}

void map::select(layer_id layer) noexcept
{
  if (has_layer(layer)) {
    m_activeLayer = layer;
  }
}

void map::add_layer()
{
  m_layers.emplace_back(m_nRows, m_nCols);
}

void map::add_row(tile_id id)
{
  for (auto& layer : m_layers) {
    layer.add_row(id);
  }
  ++m_nRows;
}

void map::add_col(tile_id id)
{
  for (auto& layer : m_layers) {
    layer.add_col(id);
  }
  ++m_nCols;
}

void map::remove_row() noexcept
{
  if (m_nRows == 1) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_row();
  }
  --m_nRows;
}

void map::remove_col() noexcept
{
  if (m_nCols == 1) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.remove_col();
  }
  --m_nCols;
}

void map::set_rows(int nRows)
{
  nRows = at_least(nRows, 1);

  if (nRows == m_nRows) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.set_rows(nRows);
  }

  m_nRows = nRows;
}

void map::set_cols(int nCols)
{
  nCols = at_least(nCols, 1);

  if (nCols == m_nCols) {
    return;
  }

  for (auto& layer : m_layers) {
    layer.set_cols(nCols);
  }

  m_nCols = nCols;
}

void map::set_visibility(layer_id layer, bool visibility) noexcept
{
  if (has_layer(layer)) {
    m_layers[static_cast<std::size_t>(layer.get())].set_visible(visibility);
  }
}

auto map::is_visible(layer_id layer) const noexcept -> bool
{
  const auto index = static_cast<std::size_t>(layer.get());
  return has_layer(layer) && m_layers[index].visible();
}

auto map::num_layers() const noexcept -> int
{
  return static_cast<int>(m_layers.size());
}

auto map::has_layer(layer_id layer) const noexcept -> bool
{
  const auto& value = layer.get();
  return value >= 0 && value < num_layers();
}

auto map::in_bounds(row r, col c) const -> bool
{
  return (r >= 0_row) && (c >= 0_col) && (r.get() < rows()) &&
         (c.get() < cols());
}

auto map::rows() const noexcept -> int
{
  return m_nRows;
}

auto map::cols() const noexcept -> int
{
  return m_nCols;
}

auto map::width() const noexcept -> int
{
  return m_nCols * m_tileSize.get();
}

auto map::height() const noexcept -> int
{
  return m_nRows * m_tileSize.get();
}

auto map::get_tile_size() noexcept -> tile_size&
{
  return m_tileSize;
}

auto map::get_tile_size() const noexcept -> const tile_size&
{
  return m_tileSize;
}

auto map::begin() const noexcept -> const_iterator
{
  return m_layers.begin();
}

auto map::end() const noexcept -> const_iterator
{
  return m_layers.end();
}

}  // namespace tactile::core
