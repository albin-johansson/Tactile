#pragma once

#include <concepts>  // invocable
#include <optional>  // optional

#include "layer.hpp"
#include "tile_size.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::core {

/**
 * @class map
 *
 * @brief Represents the main tilemaps in the application.
 *
 * @details Tilemaps are composed by multiple tile layers.
 *
 * @see `layer`
 *
 * @since 0.1.0
 *
 * @headerfile map.hpp
 */
class map final
{
 public:
  using const_iterator = vector_map<layer_id, layer>::const_iterator;

  /**
   * @brief Creates a map with one layer.
   *
   * @details The amount of rows and columns in the map is always at
   * least 1.
   *
   * @param nRows the initial number of rows in the map.
   * @param nCols the initial number of columns in the map.
   *
   * @since 0.1.0
   */
  map(row_t nRows, col_t nCols);

  template <std::invocable<layer_id, const layer&> T>
  void each_layer(T&& callable) const
  {
    for (const auto& [key, layer] : m_layers) {
      callable(key, layer);
    }
  }

  /**
   * @brief Runs a flood fill in the currently active layer.
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
   * @brief Sets the value of the tile at the specified position in the
   * currently active layer.
   *
   * @param pos the position of the tile that will be modified.
   * @param id the new value of the tile.
   *
   * @since 0.1.0
   */
  void set_tile(const position& pos, tile_id id);

  /**
   * @brief Removes all occurrences of the specified ID in all layers.
   *
   * @param id the ID that will be replaced with `empty`.
   *
   * @since 0.1.0
   */
  void remove_all(tile_id id);

  /**
   * @brief Removes all layers from the map.
   *
   * @post You must add at least one layer before invoking any layer-related
   * member function of the map object, after invoking this function.
   *
   * @since 0.1.0
   */
  void remove_layers();

  /**
   * @brief Removes the currently active layer from the map.
   *
   * @note This function has no effect if there is no active layer.
   *
   * @warning If there is an active layer, then it will be removed but no other
   * layer will be made active.
   *
   * @since 0.1.0
   */
  void remove_layer();

  /**
   * @brief Selects the tile layer associated with the specified ID.
   *
   * @note This method has no effect if the supplied ID is invalid.
   *
   * @param id the layer ID of the tile layer that will be selected.
   *
   * @since 0.1.0
   */
  void select_layer(layer_id id) noexcept;

  /**
   * @brief Adds an empty layer to the map.
   *
   * @since 0.1.0
   */
  auto add_layer() -> layer_id;

  /**
   * @brief Adds a layer to the map.
   *
   * @note This function does *not* modify the next layer ID property.
   *
   * @param id the ID that will be associated with the layer.
   * @param layer the layer that will be added.
   *
   * @since 0.1.0
   */
  void add_layer(layer_id id, layer&& layer);

  /**
   * @brief Adds a row to the map.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   *
   * @since 0.1.0
   */
  void add_row(tile_id id = empty);

  /**
   * @brief Adds a column to the map.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   *
   * @since 0.1.0
   */
  void add_col(tile_id id = empty);

  /**
   * @brief Removes a row from the map.
   *
   * @note This method has no effect if the tile map only contains one row.
   *
   * @since 0.1.0
   */
  void remove_row();

  /**
   * @brief Removes a column from the map.
   *
   * @note This method has no effect if the tile map only contains one column.
   *
   * @since 0.1.0
   */
  void remove_col();

  /**
   * @copydoc tile_size::increase()
   */
  void increase_tile_size()
  {
    m_tileSize.increase();
  }

  /**
   * @copydoc tile_size::decrease()
   */
  void decrease_tile_size() noexcept
  {
    m_tileSize.decrease();
  }

  /**
   * @copydoc tile_size::reset()
   */
  void reset_tile_size() noexcept
  {
    m_tileSize.reset();
  }

  /**
   * @brief Sets the next available layer ID.
   *
   * @pre There mustn't be a layer associated with `id`.
   *
   * @details This property is automatically incremented when calling
   * `add_layer()`. This function is meant to be used when parsing maps from
   * save files.
   *
   * @param id the ID that will be used by the next layer.
   *
   * @since 0.1.0
   */
  void set_next_layer_id(layer_id id) noexcept;

  /**
   * @brief Sets the total number of rows in the map.
   *
   * @param nRows the new number of rows in the map. Clamped to be at
   * least 1.
   *
   * @since 0.1.0
   */
  void set_rows(row_t nRows);

  /**
   * @brief Sets the total number of columns in the map.
   *
   * @param nCols the new number of columns in the map. Clamped to be at
   * least 1.
   *
   * @since 0.1.0
   */
  void set_cols(col_t nCols);

  /**
   * @brief Sets the visibility of a tile layer.
   *
   * @note This method has no effect if the specified layer ID isn't associated
   * with a tile layer.
   *
   * @param id the ID of the layer that will have its visibility changed.
   * @param visibility `true` if the layer should be visible; `false` otherwise.
   *
   * @since 0.1.0
   */
  void set_visibility(layer_id id, bool visibility);

  /**
   * @brief Returns the ID of the tile at the specified position.
   *
   * @param position the position of the tile to query.
   *
   * @return the ID of the tile at the specified position; `std::nullopt` if the
   * position is out-of-bounds.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_at(const position& position) const
      -> std::optional<tile_id>;

  /**
   * @brief Indicates whether or not the layer associated with the specified
   * ID is visible.
   *
   * @details This method returns false if the supplied ID isn't associated with
   * a tile layer. Tile layers are visible by default.
   *
   * @param id the ID of the tile layer that will be checked.
   *
   * @return `true` if the layer is visible; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto is_visible(layer_id id) const -> bool;

  /**
   * @brief Returns the amount of layers present in the map.
   *
   * @details Tilemaps are initially created with 1 tile layer.
   *
   * @return the amount of layers present in the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto layer_count() const noexcept -> int;

  /**
   * @brief Indicates whether or not the specified layer ID is associated
   * with a tile layer.
   *
   * @param id the tile layer ID that will be checked.
   *
   * @return `true` if the supplied layer ID is associated with a tile layer;
   * `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto has_layer(layer_id id) const -> bool;

  /**
   * @brief Indicates whether or not the supplied position is within the bounds
   * of the map.
   *
   * @param pos the position that will be checked.
   *
   * @return `true` if the supplied positions is in bounds; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto in_bounds(const position& pos) const -> bool;

  /**
   * @brief Returns the total number of rows in the map.
   *
   * @return the amount of rows in the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto row_count() const -> row_t;

  /**
   * @brief Returns the total number of columns in the map.
   *
   * @return the amount of columns in the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto col_count() const -> col_t;

  /**
   * @brief Returns the amount of tiles in the map.
   *
   * @return the number of tiles in the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_count() const -> int;

  /**
   * @brief Returns the pixel width of the map.
   *
   * @return the pixel width of the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto width() const -> int;

  /**
   * @brief Returns the pixel height of the map.
   *
   * @return the pixel height of the map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto height() const -> int;

  [[nodiscard]] auto get_layer(layer_id id) const -> const layer&
  {
    return m_layers.at(id);
  }

  /**
   * @brief Returns the ID of the currently active layer.
   *
   * @return the ID of the active layer; `std::nullopt` if there is no active
   * layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto active_layer_id() const noexcept -> std::optional<layer_id>
  {
    return m_activeLayer;
  }

  /**
   * @brief Returns the current tile size value.
   *
   * @return the current tile size.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto current_tile_size() const noexcept -> int
  {
    return m_tileSize.get();
  }

  /**
   * @brief Returns an iterator to the first layer.
   *
   * @return an iterator to the first layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return m_layers.begin();
  }

  /**
   * @brief Returns an iterator one-past the last layer.
   *
   * @return an iterator one-past the last layer.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return m_layers.end();
  }

 private:
  vector_map<layer_id, layer> m_layers;
  std::optional<layer_id> m_activeLayer;
  layer_id m_nextLayer{1};
  tile_size m_tileSize;

  [[nodiscard]] auto current_layer() -> layer&;

  [[nodiscard]] auto current_layer() const -> const layer&;

  [[nodiscard]] auto find_layer(layer_id id) -> layer*;

  [[nodiscard]] auto find_layer(layer_id id) const -> const layer*;
};

}  // namespace tactile::core
