#pragma once

#include <memory>
#include <vector>

#include "tactile_types.hpp"
#include "tile_layer.hpp"
#include "tile_size.hpp"
#include "type_utils.hpp"

namespace tactile::model {

/**
 * @class tilemap
 *
 * @brief Represents the main tilemaps in the application.
 *
 * @details Tilemaps are composed by multiple tile layers.
 *
 * @see `tile_layer`
 *
 * @since 0.1.0
 *
 * @headerfile tilemap.hpp
 */
class tilemap final {
 public:
  using iterator = typename std::vector<tile_layer>::iterator;
  using const_iterator = typename std::vector<tile_layer>::const_iterator;

  /**
   * @brief Creates a tilemap with one layer.
   *
   * @details The amount of rows and columns in the tilemap is always at
   * least 1.
   *
   * @param nRows the initial number of rows in the tilemap.
   * @param nCols the initial number of columns in the tilemap.
   *
   * @since 0.1.0
   */
  tilemap(int nRows, int nCols);

  /**
   * @brief Selects the tile layer associated with the specified index.
   *
   * @note This method has no effect if the supplied index is invalid.
   *
   * @param layer the layer index of the tile layer that will be selected.
   *
   * @since 0.1.0
   */
  void select(int layer) noexcept;

  /**
   * @brief Adds an empty layer to the tilemap.
   *
   * @since 0.1.0
   */
  void add_layer() noexcept;

  /**
   * @brief Adds a row to the tilemap.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   *
   * @since 0.1.0
   */
  void add_row(tile_id id = empty) noexcept;

  /**
   * @brief Adds a column to the tilemap.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   *
   * @since 0.1.0
   */
  void add_col(tile_id id = empty) noexcept;

  /**
   * @brief Removes a row from the tilemap.
   *
   * @note This method has no effect if the tile map only contains one row.
   *
   * @since 0.1.0
   */
  void remove_row() noexcept;

  /**
   * @brief Removes a column from the tilemap.
   *
   * @note This method has no effect if the tile map only contains one column.
   *
   * @since 0.1.0
   */
  void remove_col() noexcept;

  /**
   * @brief Sets the total number of rows in the tilemap.
   *
   * @param nRows the new number of rows in the tilemap. Clamped to be at
   * least 1.
   *
   * @since 0.1.0
   */
  void set_rows(int nRows) noexcept;

  /**
   * @brief Sets the total number of columns in the tilemap.
   *
   * @param nCols the new number of columns in the tilemap. Clamped to be at
   * least 1.
   *
   * @since 0.1.0
   */
  void set_cols(int nCols) noexcept;

  /**
   * @brief Sets the visibility of a tile layer.
   *
   * @note This method has no effect if the specified layer index isn't
   * associated with a tile layer.
   *
   * @param layer the index of the layer that will have its visibility changed.
   * @param visibility true if the layer should be visible; false otherwise.
   *
   * @since 0.1.0
   */
  void set_visibility(int layer, bool visibility) noexcept;

  /**
   * @brief Indicates whether or not the layer associated with the specified
   * index is visible.
   *
   * @details This method returns false if the supplied index isn't
   * associated with a tile layer. Tile layers are visible by default.
   *
   * @param layer the index of the tile layer that will be checked. An
   * invalid index results in the method returning `false`.
   *
   * @return `true` if the layer is visible; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto is_visible(int layer) const noexcept -> bool;

  /**
   * @brief Returns the amount of layers present in the tilemap.
   *
   * @deatils Tilemaps are initially created with 1 tile layer.
   *
   * @return the amount of layers present in the tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto num_layers() const noexcept -> int;

  /**
   * @brief Returns the total number of rows in the tilemap.
   *
   * @return the amount of rows in the tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto rows() const noexcept -> int;

  /**
   * @brief Returns the total number of columns in the tilemap.
   *
   * @return the amount of columns in the tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto cols() const noexcept -> int;

  /**
   * @brief Returns the pixel width of the tilemap.
   *
   * @return the pixel width of the tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto width() const noexcept -> int;

  /**
   * @brief Returns the pixel height of the tilemap.
   *
   * @return the pixel height of the tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto height() const noexcept -> int;

  /**
   * @brief Returns the current tile size.
   *
   * @return the current tile size.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_tile_size() noexcept -> tile_size&;

  /**
   * @copydoc get_tile_size()
   */
  [[nodiscard]] auto get_tile_size() const noexcept -> const tile_size&;

  /**
   * @brief Returns an iterator to the first layer.
   *
   * @return an iterator to the first layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto begin() const noexcept -> const_iterator;

  /**
   * @brief Returns an iterator one-past the last layer.
   *
   * @return an iterator one-past the last layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto end() const noexcept -> const_iterator;

 private:
  int m_nRows;
  int m_nCols;
  int m_activeLayer;
  std::vector<tile_layer> m_layers;
  tile_size m_tileSize;

  /**
   * @brief Indicates whether or not the specified layer index is associated
   * with a tile layer.
   *
   * @param layer the tile layer index that will be checked.
   *
   * @return `true` if the supplied layer index is associated with a tile layer;
   * `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto has_layer(int layer) const noexcept -> bool;
};

}  // namespace tactile::model
