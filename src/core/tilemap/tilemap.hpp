#pragma once

#include <vector>

#include "maybe.hpp"
#include "smart_pointers.hpp"
#include "tile_id.hpp"
#include "tile_layer.hpp"
#include "tile_size.hpp"
#include "tilemap_renderer.hpp"
#include "type_utils.hpp"

class QPainter;

namespace tactile {

/**
 * The <code>Tilemap</code> class represents the main tilemaps in the
 * Tactile application. tilemaps are composed by multiple tile layers.
 *
 * @see TileLayer
 * @since 0.1.0
 */
class Tilemap final {
 public:
  friend class TilemapRenderer;

  /**
   * Creates a tilemap instance with one layer. The amount of rows or
   * columns in the tilemap is always at least 1.
   *
   * @param nRows the initial number of rows in the tilemap.
   * @param nCols the initial number of columns in the tilemap.
   * @since 0.1.0
   */
  Tilemap(int nRows, int nCols);

  ~Tilemap() noexcept;

  [[nodiscard]] static Unique<Tilemap> unique(int nRows, int nCols);

  [[nodiscard]] static Shared<Tilemap> shared(int nRows, int nCols);

  /**
   * Renders the tilemap.
   *
   * @param painter the painter that will be used.
   * @since 0.1.0
   */
  void draw(QPainter& painter) const noexcept;

  /**
   * Selects the tile layer associated with the specified index. This method has
   * no effect if the supplied index is invalid.
   *
   * @param layer the layer index of the tile layer that will be selected.
   * @since 0.1.0
   */
  void select(int layer) noexcept;

  /**
   * Adds an empty layer to the tilemap.
   *
   * @since 0.1.0
   */
  void add_layer() noexcept;

  /**
   * Adds a row to the tilemap.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   * @since 0.1.0
   */
  void add_row(tile_id id = empty) noexcept;

  /**
   * Adds a column to the tilemap.
   *
   * @param id the tile ID that the new tiles will have, defaults to empty.
   * @since 0.1.0
   */
  void add_col(tile_id id = empty) noexcept;

  /**
   * Removes a row from the tilemap. This method has no effect if the tile
   * map only contains one row.
   *
   * @since 0.1.0
   */
  void remove_row() noexcept;

  /**
   * Removes a column from the tilemap. This method has no effect if the tile
   * map only contains one column.
   *
   * @since 0.1.0
   */
  void remove_col() noexcept;

  /**
   * Sets the total number of rows in the tilemap.
   *
   * @param nRows the new number of rows in the tilemap. Clamped to be at
   * least 1.
   * @since 0.1.0
   */
  void set_rows(int nRows) noexcept;

  /**
   * Sets the total number of columns in the tilemap.
   *
   * @param nCols the new number of columns in the tilemap. Clamped to be at
   * least 1.
   * @since 0.1.0
   */
  void set_cols(int nCols) noexcept;

  /**
   * Sets the visibility of a tile layer. This method has no effect if the
   * specified layer index isn't associated with a tile layer.
   *
   * @param layer the index of the layer that will have its visibility changed.
   * @param visibility true if the layer should be visible; false otherwise.
   * @since 0.1.0
   */
  void set_visibility(int layer, bool visibility) noexcept;

  /**
   * Indicates whether or not the layer associated with the specified index is
   * visible. This method returns false if the supplied index isn't associated
   * with a tile layer. Tile layers are visible by default.
   *
   * @param layer the index of the tile layer that will be checked. An invalid
   * index results in the method returning false.
   * @return true if the layer is visible; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool is_visible(int layer) const noexcept;

  /**
   * Returns the amount of layers present in the tilemap. tilemaps are created
   * with 1 tile layer.
   *
   * @return the amount of layers present in the tilemap.
   * @since 0.1.0
   */
  [[nodiscard]] int get_layer_amount() const noexcept;

  /**
   * Returns the total number of rows in the tilemap.
   *
   * @return the amount of rows in the tilemap.
   * @since 0.1.0
   */
  [[nodiscard]] int rows() const noexcept { return m_nRows; }

  /**
   * Returns the total number of columns in the tilemap.
   *
   * @return the amount of columns in the tilemap.
   * @since 0.1.0
   */
  [[nodiscard]] int cols() const noexcept { return m_nCols; }

  [[nodiscard]] int width() const noexcept
  {
    return m_nCols * m_tileSize.get();
  }

  [[nodiscard]] int height() const noexcept
  {
    return m_nRows * m_tileSize.get();
  }

  [[nodiscard]] tile_size& get_tile_size() noexcept { return m_tileSize; }

  [[nodiscard]] const tile_size& get_tile_size() const noexcept
  {
    return m_tileSize;
  }

 private:
  int m_nRows;
  int m_nCols;
  int m_activeLayer;
  std::vector<TileLayer> m_layers;
  TilemapRenderer m_renderer;
  tile_size m_tileSize;

  /**
   * Indicates whether or not the specified layer index is associated with a
   * tile layer.
   *
   * @param layer the tile layer index that will be checked.
   * @return true if the supplied layer index is associated with a tile layer;
   * false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool has_layer(int layer) const noexcept;
};

static_assert(validate<Tilemap>());

}  // namespace tactile
