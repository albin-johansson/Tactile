#include "map_document.hpp"

#include "add_col.hpp"
#include "add_row.hpp"
#include "bucket_fill.hpp"
#include "erase_sequence.hpp"
#include "remove_col.hpp"
#include "remove_row.hpp"
#include "resize_map.hpp"
#include "stamp_sequence.hpp"

namespace tactile::core {

map_document::map_document(QObject* parent)
    : QObject{parent},
      m_map{std::make_unique<map>(5_row, 5_col)},
      m_tilesets{std::make_unique<tileset_manager>()},
      m_commands{new command_stack{this}}
{
  m_commands->setUndoLimit(100);

  connect(m_commands,
          &command_stack::canUndoChanged,
          this,
          &map_document::undo_state_updated);

  connect(m_commands,
          &command_stack::canRedoChanged,
          this,
          &map_document::redo_state_updated);

  connect(m_commands,
          &command_stack::undoTextChanged,
          this,
          &map_document::undo_text_updated);

  connect(m_commands,
          &command_stack::redoTextChanged,
          this,
          &map_document::redo_text_updated);
}

void map_document::undo()
{
  m_commands->undo();
}

void map_document::redo()
{
  m_commands->redo();
}

void map_document::flood(const position& position,
                         tile_id target,
                         tile_id replacement)
{
  m_commands->push<cmd::bucket_fill>(
      m_map.get(), position, target, replacement);
}

void map_document::add_stamp_sequence(vector_map<position, tile_id>&& oldState,
                                      vector_map<position, tile_id>&& sequence)
{
  m_commands->push<cmd::stamp_sequence>(
      m_map.get(), std::move(oldState), std::move(sequence));
}

void map_document::add_erase_sequence(vector_map<position, tile_id>&& oldState)
{
  m_commands->push<cmd::erase_sequence>(m_map.get(), std::move(oldState));
}

void map_document::add_row()
{
  m_commands->push<cmd::add_row>(m_map.get());
}

void map_document::add_column()
{
  m_commands->push<cmd::add_col>(m_map.get());
}

void map_document::remove_row()
{
  m_commands->push<cmd::remove_row>(m_map.get());
}

void map_document::remove_column()
{
  m_commands->push<cmd::remove_col>(m_map.get());
}

void map_document::resize(row_t nRows, col_t nCols)
{
  m_commands->push<cmd::resize_map>(m_map.get(), nRows, nCols);
}

auto map_document::add_tileset(const QImage& image,
                               const QString& path,
                               const QString& name,
                               tile_width tileWidth,
                               tile_height tileHeight)
    -> std::optional<tileset_id>
{
  if (!image.isNull()) {
    const auto id = m_tilesets->emplace(image, tileWidth, tileHeight);
    auto& tileset = m_tilesets->at(id);
    tileset.set_name(name);
    tileset.set_path(path);
    return id;
  } else {
    return std::nullopt;
  }
}

void map_document::remove_tileset(tileset_id id)
{
  Q_ASSERT(m_tilesets->contains(id));

  const auto [first, last] = m_tilesets->range_of(id);

  for (auto i = first; i < last; ++i) {
    m_map->remove_all(i);
  }

  m_tilesets->remove(id);
}

void map_document::select_tileset(tileset_id id)
{
  m_tilesets->select(id);
}

void map_document::set_selection(position topLeft, position bottomRight)
{
  m_tilesets->set_selection(topLeft, bottomRight);
}

void map_document::select_layer(layer_id id)
{
  m_map->select_layer(id);
}

void map_document::increase_tile_size()
{
  m_map->get_tile_size().increase();
}

void map_document::decrease_tile_size()
{
  m_map->get_tile_size().decrease();
}

void map_document::reset_tile_size()
{
  m_map->get_tile_size().reset();
}

auto map_document::can_undo() const -> bool
{
  return m_commands->canUndo();
}

auto map_document::can_redo() const -> bool
{
  return m_commands->canRedo();
}

auto map_document::undo_text() const -> QString
{
  return m_commands->undoText();
}

auto map_document::redo_text() const -> QString
{
  return m_commands->redoText();
}

}  // namespace tactile::core
