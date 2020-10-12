#pragma once

#include <cassert>   // assert
#include <concepts>  // invocable
#include <optional>  // optional
#include <vector>    // vector

#include "position.hpp"
#include "types.hpp"

namespace tactile::core {

/**
 * @class layer
 *
 * @brief Represents a layer of tiles in a map.
 *
 * @since 0.1.0
 *
 * @see `map`
 *
 * @headerfile layer.hpp
 */
class layer final
{
  using tile_row = std::vector<tile_id>;
  using tile_matrix = std::vector<tile_row>;

 public:
  /**
   * @brief Creates a tile layer with the specified dimensions.
   *
   * @details All of the tiles in the created layer are initialized with the
   * value of the `empty` tile ID.
   *
   * @param nRows the initial number of rows in the layer; clamped to 1 if
   * the supplied value is less than 1.
   * @param nCols the initial number of columns in the layer; clamped to 1 if
   * the supplied value is less than 1.
   *
   * @since 0.1.0
   */
  layer(row_t nRows, col_t nCols);

  /**
   * @brief Iterates each tile in the layer.
   *
   * @tparam T the type of the function object that will be invoked.
   *
   * @param callable the callable that will be invoked for each tile in the
   * layer.
   *
   * @since 0.1.0
   */
  template <std::invocable<tile_id> T>
  void for_each(T&& callable) const
  {
    const auto endRow = row_count();
    const auto endCol = col_count();
    for (row_t row{0}; row < endRow; ++row) {
      for (col_t col{0}; col < endCol; ++col) {
        callable(m_tiles.at(row.get()).at(col.get()));
      }
    }
  }

  /**
   * @brief Runs a flood fill in the layer.
   *
   * @param origin the starting position of the flood fill.
   * @param replacement the tile type that will be used as the replacement.
   * @param[out] positions the vector that the affected positions will be added
   * to.
   *
   * @since 0.1.0
   */
  void flood(const position& origin,
             tile_id replacement,
             std::vector<position>& positions);

  /**
   * @brief Removes all occurrences of the specified ID in the layer.
   *
   * @param id the ID that will be replaced with `empty`.
   *
   * @since 0.1.0
   */
  void remove_all(tile_id id);

  /**
   * @brief Adds a row to the tile layer.
   *
   * @param id the tile ID of all of the new cells in the added row.
   *
   * @since 0.1.0
   */
  void add_row(tile_id id);

  /**
   * @brief Adds a column to the tile layer.
   *
   * @param id the tile ID of all of the new cells in the added column.
   *
   * @since 0.1.0
   */
  void add_col(tile_id id);

  /**
   * @brief Removes a row from the tile layer.
   *
   * @note The tile layer will always have at least one row, so this method
   * has no effect if the tile layer has exactly one row.
   *
   * @since 0.1.0
   */
  void remove_row() noexcept;

  /**
   * @brief Removes a column from the tile layer.
   *
   * @note The tile layer will always have at least one column, so this
   * method has no effect if the tile layer has exactly one column.
   *
   * @since 0.1.0
   */
  void remove_col() noexcept;

  /**
   * @brief Sets the total number of rows in the layer.
   *
   * @pre `nRows` must be greater than or equal to 1.
   *
   * @param nRows the new number of rows in the layer.
   *
   * @since 0.1.0
   */
  void set_rows(row_t nRows);

  /**
   * @brief Sets the total number of columns in the layer.
   *
   * @pre `nCols` must be greater than or equal to 1.
   *
   * @param nCols the new number of columns in the layer.
   *
   * @since 0.1.0
   */
  void set_cols(col_t nCols);

  /**
   * @brief Sets the tile ID of a tile in the tile layer.
   *
   * @note This method has no effect if the supplied position is out-of-bounds.
   *
   * @param pos the position of the tile that will be changed.
   * @param id the new tile ID.
   *
   * @since 0.1.0
   */
  void set_tile(const position& pos, tile_id id) noexcept;

  /**
   * @brief Sets whether or not the tile layer is visible.
   *
   * @param visible `true` if the tile layer should be visible; `false`
   * otherwise.
   *
   * @since 0.1.0
   */
  void set_visible(bool visible) noexcept;

  /**
   * @brief Returns the number of rows in the tile layer.
   *
   * @note A tile layer always contains at least one row.
   *
   * @return the number of rows in the tile layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto row_count() const noexcept -> row_t;

  /**
   * @brief Returns the number of columns in the tile layer.
   *
   * @note A tile layer always contains at least one column.
   *
   * @return the number of columns in the tile layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto col_count() const noexcept -> col_t;

  /**
   * @brief Returns the total amount of tiles in the layer.
   *
   * @return the number of tiles in the layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_count() const noexcept -> int;

  /**
   * @brief Returns the ID of the tile at the specified position.
   *
   * @note This method returns `std::nullopt` if the supplied position
   * is out-of-bounds.
   *
   * @param pos the position to obtain the tile ID of.
   *
   * @return the ID of the tile at the specified position; `std::nullopt` if the
   * position is out-of-bounds.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_at(const position& pos) const
      -> std::optional<tile_id>;

  /**
   * @brief Indicates whether or not the specified position is in bounds of the
   * tile layer.
   *
   * @param pos the position that will be checked.
   *
   * @return `true` if the position is in bounds; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto in_bounds(const position& pos) const noexcept -> bool;

  /**
   * @brief Indicates whether or not the tile layer is visible.
   *
   * @note Tile layers are visible by default.
   *
   * @return `true` if the tile layer is visible; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto visible() const noexcept -> bool
  {
    return m_visible;
  }

 private:
  tile_matrix m_tiles;
  bool m_visible{true};
};

}  // namespace tactile::core
