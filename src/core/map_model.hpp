#pragma once

#include <qobject.h>
#include <qpainter.h>

#include <utility>  // forward

#include "command_stack.hpp"
#include "map.hpp"
#include "small_map.hpp"
#include "tileset.hpp"

namespace tactile::core {

class tileset_model;

class map_model final : public QObject  // TODO rename to map_document
{
  Q_OBJECT

 public:
  explicit map_model(QObject* parent = nullptr);

  /// @name Commands
  /// @{

  void undo();

  void redo();

  void flood(const core::position& position,
             tile_id target,
             tile_id replacement);

  void add_stamp_sequence(small_map<core::position, tile_id>&& oldState,
                          small_map<core::position, tile_id>&& sequence);

  void add_erase_sequence(small_map<core::position, tile_id>&& oldState,
                          std::vector<core::position>&& positions);

  void add_row();

  void add_column();

  void remove_row();

  void remove_column();

  void resize(int nRows, int nCols);

  [[nodiscard]] auto can_undo() const -> bool;

  [[nodiscard]] auto can_redo() const -> bool;

  [[nodiscard]] auto undo_text() const -> QString;

  [[nodiscard]] auto redo_text() const -> QString;

  /// @}

  /// @name Tilemap queries
  /// @{

  [[nodiscard]] auto rows() const noexcept -> int;

  [[nodiscard]] auto columns() const noexcept -> int;

  [[nodiscard]] auto width() const noexcept -> int;

  [[nodiscard]] auto height() const noexcept -> int;

  /// @}

  /// @name Layer handling
  /// @{

  void select_layer(layer_id layer);

  [[nodiscard]] auto has_layer(layer_id layer) const noexcept -> bool;

  /// @}

  /// @name Tile size
  /// @{

  void reset_tile_size();

  void increase_tile_size();

  void decrease_tile_size();

  [[nodiscard]] auto current_tile_size() const noexcept -> int;

  /// @}

  [[nodiscard]] auto get_map() noexcept -> map&;

  [[nodiscard]] auto get_map() const noexcept -> const map&;

  [[nodiscard]] auto get() noexcept -> map*;

  [[nodiscard]] auto get() const noexcept -> const map*;

 signals:
  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

 private:
  std::unique_ptr<map> m_map;
  command_stack* m_commands;
};

}  // namespace tactile::core
