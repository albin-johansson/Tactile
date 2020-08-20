#pragma once

#include <cstddef>  // size_t

namespace tactile::model {
namespace {

[[nodiscard]] constexpr auto clamp_component(int comp) noexcept -> int
{
  return (comp < 0) ? 0 : comp;
}

}  // namespace

/**
 * @class map_position
 *
 * @brief Represents a row- and column-based position in a tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile map_position.hpp
 */
class map_position final {
 public:
  constexpr map_position() noexcept = default;

  /**
   * @brief Creates a map position.
   *
   * @param row the row value of the map position, clamped to zero if negative.
   * @param col the column value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr map_position(int row, int col) noexcept
      : m_row{clamp_component(row)}, m_col{clamp_component(col)}
  {}

  /**
   * @brief Sets the row coordinate of the map position.
   *
   * @param row the new row value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr void set_row(int row) noexcept { m_row = clamp_component(row); }

  /**
   * @brief Sets the column coordinate of the map position.
   *
   * @param row the new column value of the map position, clamped to zero if
   * negative.
   *
   * @since 0.1.0
   */
  constexpr void set_col(int col) noexcept { m_col = clamp_component(col); }

  /**
   * @brief Returns a map position that is one step north of this map position.
   *
   * @return a map position that is one step north of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto north() const noexcept -> map_position
  {
    return {m_row - 1, m_col};
  }

  /**
   * @brief Returns a map position that is one step to the east of this map
   * position.
   *
   * @return a map position that is one step to the east of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto east() const noexcept -> map_position
  {
    return {m_row, m_col + 1};
  }

  /**
   * @brief Returns a map position that is one step south of this map position.
   *
   * @return a map position that is one step south of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto south() const noexcept -> map_position
  {
    return {m_row + 1, m_col};
  }

  /**
   * @brief Returns a map position that is one step to the west of this map
   * position.
   *
   * @return a map position that is one step to the west of this map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto west() const noexcept -> map_position
  {
    return {m_row, m_col - 1};
  }

  /**
   * @brief Returns the row index of the map position.
   *
   * @note The returned value is never negative.
   *
   * @return the row index of the map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto row() const noexcept -> int { return m_row; }

  /**
   * @brief Returns the column index of the map position.
   *
   * @note The returned value is never negative.
   *
   * @return the column index of the map position.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto col() const noexcept -> int { return m_col; }

  /**
   * @brief Returns the row index associated with the map position.
   *
   * @details This function is meant to be used when indexing vectors, etc.
   *
   * @return the row index.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto urow() const noexcept -> std::size_t
  {
    return static_cast<std::size_t>(m_row);
  }

  /**
   * @brief Returns the column index associated with the map position.
   *
   * @details This function is meant to be used when indexing vectors, etc.
   *
   * @return the column index.
   *
   * @since 0.1.0
   */
  [[nodiscard]] constexpr auto ucol() const noexcept -> std::size_t
  {
    return static_cast<std::size_t>(m_col);
  }

 private:
  int m_row{};
  int m_col{};
};

/**
 * @brief Indicates whether or not two map positions are the same.
 *
 * @param lhs the left-hand side map position.
 * @param rhs the right-hand side map position.
 *
 * @return `true` if the map positions have the same coordinates; `false`
 * otherwise.
 *
 * @since 0.1.0
 */
[[nodiscard]] inline constexpr auto operator==(const map_position& lhs,
                                               const map_position& rhs) noexcept
    -> bool
{
  return (lhs.row() == rhs.row()) && (lhs.col() == rhs.col());
}

/**
 * @brief Indicates whether or not two map positions aren't the same.
 *
 * @param lhs the left-hand side map position.
 * @param rhs the right-hand side map position.
 *
 * @return `true` if the map positions don't have the same coordinates; `false`
 * otherwise.
 *
 * @since 0.1.0
 */
[[nodiscard]] inline constexpr auto operator!=(const map_position& lhs,
                                               const map_position& rhs) noexcept
    -> bool
{
  return !(lhs == rhs);
}

}  // namespace tactile::model
