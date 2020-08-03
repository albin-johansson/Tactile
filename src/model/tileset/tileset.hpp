#pragma once

#include <QImage>
#include <memory>
#include <set>
#include <string_view>

#include "tactile_types.hpp"
#include "type_utils.hpp"

namespace tactile::model {

/**
 * @class tileset
 *
 * @brief Represents a collection of tiles in a sprite sheet.
 *
 * @details A tileset is really just an image that contains a set of tiles
 * that are used to build tilemaps, where all tilesets store their first and
 * last valid tile identifiers. Tilesets must be created from images that
 * store their sprites aligned in a grid. However, the tiles don't
 * necessarily have to be be square.
 *
 * @since 0.1.0
 *
 * @headerfile tileset.hpp
 */
class tileset final {
 public:
  /**
   * @brief Creates a tileset with the initial first ID set to 1.
   *
   * @details Both the supplied width and height will be adjusted to be at
   * least 1.
   *
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @throws tactile_error if the supplied image is null.
   *
   * @since 0.1.0
   */
  tileset(const QImage& image, int tileWidth, int tileHeight);

  /**
   * @brief Creates a tileset with the initial first ID set to 1.
   *
   * @details Both the supplied width and height will be adjusted to be at
   * least 1.
   *
   * @param path the path to the image that contains the tile sprites.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @throws tactile_error if the tileset cannot be created.
   *
   * @since 0.1.0
   */
  tileset(const QString& path, int tileWidth, int tileHeight);

  /**
   * @brief Sets the first tile ID property of the tileset.
   *
   * @details This method has no effect if the supplied ID isn't greater than
   * zero.
   *
   * @note This method doesn't check if the supplied tile ID isn't already
   * taken by another tileset.
   *
   * @param firstID the new first tile ID, must be greater than zero.
   *
   * @since 0.1.0
   */
  void set_first_id(tile_id firstID) noexcept;

  /**
   * @brief Selects the tile at the specified coordinates.
   *
   * @details Multiple tiles can be selected simultaneously.
   *
   * @note This method has no effect if the supplied position is out-of-bounds.
   *
   * @param x the x-coordinate of the selection.
   * @param y the y-coordinate of the selection.
   *
   * @since 0.1.0
   */
  void select(int x, int y) noexcept;

  /**
   * @brief Clears any previously selected tiles.
   *
   * @since 0.1.0
   */
  void clear_selection() noexcept;

  /**
   * @brief Indicates whether or not the tileset contains the specified tile ID.
   *
   * @param id the tile ID that will be checked.
   *
   * @return `true` if the tileset contains the tile ID; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto contains(tile_id id) const noexcept -> bool;

  /**
   * @brief Returns the tile ID of the tile at the specified coordinates.
   *
   * @param x the x-coordinate to check.
   * @param y the y-coordinate to check.
   *
   * @return the tile ID of the tile at the specified position; `empty` if no
   * tile was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_at(int x, int y) const noexcept -> tile_id;

  /**
   * @brief Returns the width of the tileset image.
   *
   * @return the width of the tileset image.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto width() const noexcept -> int;

  /**
   * @brief Returns the height of the tileset image.
   *
   * @return the height of the tileset image.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto height() const noexcept -> int;

  /**
   * @brief Returns the total number of rows of tiles in the tileset.
   *
   * @return the total number of rows in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto rows() const noexcept -> int;

  /**
   * @brief Returns the total number of columns of tiles in the tileset.
   *
   * @return the total number of columns in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto cols() const noexcept -> int;

  /**
   * @brief Returns the total number of tiles contained in the tileset.
   *
   * @return the total number of tiles contained in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tiles() const noexcept -> int;

  /**
   * @brief Returns the tile ID of the first tile in the tileset.
   *
   * @details The default first tile ID is `1`.
   *
   * @return the ID of the first tile in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto first_id() const noexcept -> tile_id;

  /**
   * @brief Returns the last valid tile ID associated with the tileset.
   *
   * @return the last valid ID associated with the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto last_id() const noexcept -> tile_id;

  /**
   * @brief Returns the width of the tile sprites in the tileset.
   *
   * @note The returned value is at least 1.
   *
   * @return the width of the tile sprites in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_width() const noexcept -> int;

  /**
   * @brief Returns the height of the tile sprites in the tileset.
   *
   * @note The returned value is at least 1.
   *
   * @return the height of the tile sprites in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_height() const noexcept -> int;

  /**
   * @brief Returns the currently selected tile IDs.
   *
   * @return the currently selected tile IDs.
   *
   * @since 0.1.0
   */
  [[nodiscard, deprecated]] auto selection() -> const std::set<tile_id>&;

 private:
  QImage m_sheet;  // FIXME this field might be unnecessary
  tile_id m_firstID{1};
  std::set<tile_id> m_selection;
  int m_rows;
  int m_cols;
  int m_tileWidth;
  int m_tileHeight;
  int m_nTiles;
};

static_assert(std::is_final_v<tileset>);

static_assert(std::is_move_constructible_v<tileset>);
static_assert(std::is_move_assignable_v<tileset>);

static_assert(std::is_copy_constructible_v<tileset>);
static_assert(std::is_copy_assignable_v<tileset>);

}  // namespace tactile::model
