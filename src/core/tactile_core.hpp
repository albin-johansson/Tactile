#pragma once

#include <QObject>
#include <memory>
#include <optional>
#include <unordered_map>

#include "core_fwd.hpp"
#include "tactile_types.hpp"

class QPainter;

namespace tactile {

/**
 * @class tactile_core
 *
 * @brief Represents the main interface for the core model of the application.
 *
 * @details This class is the only `QObject` of all of the core components.
 *
 * @note All tilemap mutating methods in this class have no effect if there
 * is no active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile tactile_core.hpp
 */
class tactile_core final : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Creates an tactile core instance.
   *
   * @note There should only be one instance of this class for every run of
   * the application.
   *
   * @since 0.1.0
   */
  tactile_core();

  /**
   * @copydoc tactile_core()
   */
  [[nodiscard]] static auto unique() -> std::unique_ptr<tactile_core>;

  void open_map(czstring fileName);  // TODO support TMX

  void save_as(czstring fileName) const;  // TODO implement

  /**
   * @brief Adds a tileset based on the supplied image.
   *
   * @note This method has no effect if the tileset cannot be added.
   *
   * @param fileName the image that contains the tile images.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @return the ID of the tileset that was added; `std::nullopt` if no tile
   * sheet was added.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto add_tileset(const std::shared_ptr<QImage>& image,
                                 int tileWidth,
                                 int tileHeight) noexcept -> std::optional<int>;

  /**
   * @brief Sets the number of rows in the tilemap.
   *
   * @note The amount of rows will always be at least 1.
   *
   * @param nRows the new amount of rows in the tilemap.
   *
   * @since 0.1.0
   */
  void set_rows(int nRows) noexcept;

  /**
   * @brief Sets the number of columns in the tilemap.
   *
   * @note The amount of columns will always be at least 1.
   *
   * @param nCols the new amount of columns in the tilemap.
   *
   * @since 0.1.0
   */
  void set_cols(int nCols) noexcept;

  /**
   * @brief Selects the tile layer associated with the specified index.
   *
   * @note This method has no effect if the supplied index isn't associated
   * with a tile layer.
   *
   * @param index the index of the tile layer that will be selected.
   *
   * @since 0.1.0
   */
  void select_layer(int index) noexcept;

  /**
   * @brief Selects the tilemap associated with the specified id.
   *
   * @note This method has no effect if the supplied key isn't associated
   * with a tilemap.
   *
   * @param id the key of the tilemap that will be selected.
   *
   * @since 0.1.0
   */
  void select_map(int id) noexcept;

  /**
   * @brief Returns the amount of rows in the active tilemap.
   *
   * @return the amount of rows in the active tilemap; `std::nullopt` if there
   * is no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto rows() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the amount of columns in the active tilemap.
   *
   * @return the amount of columns in the active tilemap; `std::nullopt` if
   * there is no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto cols() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the current width of the active tilemap.
   *
   * @return the current width of the active tilemap; `std::nullopt` if there is
   * no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto map_width() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the current height of the active tilemap.
   *
   * @return the current height of the active tilemap; `std::nullopt` if there
   * is no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto map_height() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the size of the tiles in the currently active tilemap.
   *
   * @return the size of the tiles in the currently active tilemap;
   * `std::nullopt` if there is no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_size() const noexcept -> std::optional<int>;

 signals:
  /**
   * @brief A signal that is emitted when the state of the active tilemap has
   * changed.
   *
   * @since 0.1.0
   */
  void s_updated();

 public slots:
  /**
   * @brief Renders the currently active tile map.
   *
   * @note This method has no effect if there is no active tilemap.
   *
   * @param painter the painter that will be used to render the tilemap.
   *
   * @since 0.1.0
   */
  void on_draw(QPainter& painter) const noexcept;

  /**
   * @brief Starts working on a new tilemap.
   *
   * @details The new tilemap will be made active.
   *
   * @note This method has no effect if the supplied ID is already associated
   * with a tilemap.
   *
   * @param id the integer that will be associated with the tilemap, must be
   * unique.
   *
   * @since 0.1.0
   */
  void on_new_map(int id) noexcept;

  /**
   * @brief Closes the map associated with the specified ID.
   *
   * @param id the ID of the tilemap that will be closed.
   *
   * @since 0.1.0
   */
  void on_close_map(int id) noexcept;

  /**
   * @brief Adds a row to the currently active tilemap.
   *
   * @since 0.1.0
   */
  void on_add_row() noexcept;

  /**
   * @brief Adds a column to the currently active tilemap.
   *
   * @since 0.1.0
   */
  void on_add_col() noexcept;

  /**
   * @brief Removes a row from the currently active tilemap.
   *
   * @since 0.1.0
   */
  void on_remove_row() noexcept;

  /**
   * @brief Removes a column from the currently active tilemap.
   *
   * @since 0.1.0
   */
  void on_remove_col() noexcept;

  /**
   * @brief Increases the tile size that is being used by the currently active
   * tilemap.
   *
   * @since 0.1.0
   */
  void on_increase_tile_size() noexcept;

  /**
   * @brief Decreases the tile size that is being used by the currently active
   * tilemap.
   *
   * @since 0.1.0
   */
  void on_decrease_tile_size() noexcept;

  /**
   * @brief Resets the tile size that is being used by the currently active
   * tilemap to its default value.
   *
   * @since 0.1.0
   */
  void on_reset_tile_size() noexcept;

 private:
  std::optional<int> m_activeMapIndex;
  std::unordered_map<int, std::unique_ptr<tilemap>> m_maps;
  std::unique_ptr<tileset_manager> m_sheetManager;

  /**
   * @brief Returns a pointer to the currently active map.
   *
   * @details You should check that the returned pointer isn't null before
   * using it.
   *
   * @return a pointer to the currently active tilemap; `nullptr` if there is
   * none.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto active_map() noexcept -> tilemap*;

  /**
   * @copydoc active_map()
   */
  [[nodiscard]] auto active_map() const noexcept -> const tilemap*;

  /**
   * @brief A templated helper method for queries that might return a value
   * from the active tilemap.
   *
   * @tparam Ret the type of the return value.
   * @tparam Functor the type of the function object.
   *
   * @param fun the function object that performs the query, has to take
   * a reference to a `tilemap` as a parameter.
   *
   * @return either a value of the return type, or `std::nullopt`.
   *
   * @since 0.1.0
   */
  template <typename Ret, typename Functor>
  [[nodiscard]] auto maybe_get(Functor fun) const -> std::optional<Ret>
  {
    if (auto* map = active_map(); map) {
      return fun(*map);
    } else {
      return std::nullopt;
    }
  }
};

}  // namespace tactile