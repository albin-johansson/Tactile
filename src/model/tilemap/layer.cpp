#include "layer.hpp"

#include <cassert>  // assert
#include <cmath>    // abs

#include "algorithm.hpp"
#include "flood_fill.hpp"

namespace tactile::model {
namespace {

[[nodiscard]] auto create_row(int nCols, tile_id value = empty)
    -> std::vector<tile_id>
{
  std::vector<tile_id> row;
  row.reserve(nCols);
  row.assign(nCols, value);
  return row;
}

}  // namespace

layer::layer(int nRows, int nCols)
{
  nRows = at_least(nRows, 1);
  nCols = at_least(nCols, 1);

  m_tiles.reserve(nRows);
  m_tiles.assign(nRows, create_row(nCols));

  assert(rows() == nRows);
  assert(cols() == nCols);
}

void layer::flood(const position& pos,
                  tile_id target,
                  tile_id replacement)
{
  flood_fill(*this, position, target, replacement);
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

void layer::set_rows(int nRows)
{
  assert(nRows >= 1);

  const auto current = rows();

  if (nRows == current) {
    return;
  }

  const auto diff = std::abs(current - nRows);

  if (current < nRows) {
    do_n(diff, [this] { add_row(empty); });
  } else {
    do_n(diff, [this]() noexcept { remove_row(); });
  }
}

void layer::set_cols(int nCols)
{
  assert(nCols >= 1);

  const auto current = cols();

  if (nCols == current) {
    return;
  }

  const auto diff = std::abs(current - nCols);

  if (current < nCols) {
    do_n(diff, [this] { add_col(empty); });
  } else {
    do_n(diff, [this]() noexcept { remove_col(); });
  }
}

void layer::set_tile(const position& pos, tile_id id) noexcept
{
  if (in_bounds(position)) {
    m_tiles[position.urow()][position.ucol()] = id;
  }
}

void layer::set_visible(bool visible) noexcept
{
  m_visible = visible;
}

auto layer::rows() const noexcept -> int
{
  return static_cast<int>(m_tiles.size());
}

auto layer::cols() const noexcept -> int
{
  assert(!m_tiles.empty());
  return static_cast<int>(m_tiles[0].size());
}

auto layer::tile_at(const position& pos) const
    -> std::optional<tile_id>
{
  if (in_bounds(position)) {
    return m_tiles[position.urow()][position.ucol()];
  } else {
    return std::nullopt;
  }
}

auto layer::in_bounds(const position& pos) const noexcept -> bool
{
  const auto row = position.urow();
  return (row < m_tiles.size()) && (position.ucol() < m_tiles[row].size());
}

auto layer::visible() const noexcept -> bool
{
  return m_visible;
}

}  // namespace tactile::model
