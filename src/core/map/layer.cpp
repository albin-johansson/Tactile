#include "layer.hpp"

#include <cassert>  // assert
#include <cmath>    // abs

#include "algorithm.hpp"
#include "flood_fill.hpp"

namespace tactile::core {
namespace {

[[nodiscard]] auto create_row(col_t nCols, tile_id value = empty)
    -> std::vector<tile_id>
{
  std::vector<tile_id> row;
  row.reserve(nCols.get());
  row.assign(nCols.get(), value);
  return row;
}

}  // namespace

layer::layer(row_t nRows, col_t nCols)
{
  nRows = at_least(nRows, 1_row);
  nCols = at_least(nCols, 1_col);

  m_tiles.reserve(nRows.get());
  m_tiles.assign(nRows.get(), create_row(nCols));

  assert(rows() == nRows);
  assert(cols() == nCols);
}

void layer::flood(const position& pos, tile_id target, tile_id replacement)
{
  flood_fill(*this, pos, target, replacement);
}

void layer::remove_all(tile_id id)
{
  const auto nRows = rows().get();
  const auto nCols = cols().get();
  for (auto r = 0; r < nRows; ++r) {
    for (auto c = 0; c < nCols; ++c) {
      if (m_tiles[r][c] == id) {
        m_tiles[r][c] = empty;
      }
    }
  }
}

void layer::add_row(tile_id id)
{
  m_tiles.push_back(create_row(cols(), id));
}

void layer::add_col(tile_id id)
{
  for (auto& row : m_tiles) {
    row.push_back(id);
  }
}

void layer::remove_row() noexcept
{
  if (m_tiles.size() > 1) {
    m_tiles.pop_back();
  }
}

void layer::remove_col() noexcept
{
  for (auto& row : m_tiles) {
    if (row.size() > 1) {
      row.pop_back();
    }
  }
}

void layer::set_rows(row_t nRows)
{
  assert(nRows >= 1_row);

  const auto current = rows();

  if (nRows == current) {
    return;
  }

  const auto diff = std::abs(current.get() - nRows.get());

  if (current < nRows) {
    do_n(diff, [this] { add_row(empty); });
  } else {
    do_n(diff, [this]() noexcept { remove_row(); });
  }
}

void layer::set_cols(col_t nCols)
{
  assert(nCols >= 1_col);

  const auto current = cols();

  if (nCols == current) {
    return;
  }

  const auto diff = std::abs(current.get() - nCols.get());

  if (current < nCols) {
    do_n(diff, [this] { add_col(empty); });
  } else {
    do_n(diff, [this]() noexcept { remove_col(); });
  }
}

void layer::set_tile(const position& pos, tile_id id) noexcept
{
  if (in_bounds(pos)) {
    m_tiles[pos.row_index()][pos.col_index()] = id;
  }
}

void layer::set_visible(bool visible) noexcept
{
  m_visible = visible;
}

auto layer::rows() const noexcept -> row_t
{
  return row_t{static_cast<int>(m_tiles.size())};
}

auto layer::cols() const noexcept -> col_t
{
  assert(!m_tiles.empty());
  return col_t{static_cast<int>(m_tiles[0].size())};
}

auto layer::tile_at(const position& pos) const -> std::optional<tile_id>
{
  if (in_bounds(pos)) {
    return m_tiles[pos.row_index()][pos.col_index()];
  } else {
    return std::nullopt;
  }
}

auto layer::in_bounds(const position& pos) const noexcept -> bool
{
  const auto row = pos.row_index();
  return (row < m_tiles.size()) && (pos.col_index() < m_tiles[row].size());
}

}  // namespace tactile::core
