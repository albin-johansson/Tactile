#include "model.hpp"

#include "tileset.hpp"

namespace tactile::core {

model::model() : m_maps{std::make_unique<map_manager>()}, m_tools{this}
{
  connect(m_maps.get(),
          &map_manager::undo_state_updated,
          this,
          &model::undo_state_updated);
  connect(m_maps.get(),
          &map_manager::redo_state_updated,
          this,
          &model::redo_state_updated);
  connect(m_maps.get(),
          &map_manager::undo_text_updated,
          this,
          &model::undo_text_updated);
  connect(m_maps.get(),
          &map_manager::redo_text_updated,
          this,
          &model::redo_text_updated);
  connect(m_maps.get(), &map_manager::added_tileset, [this](tileset_id id) {
    const auto& tileset = current_document()->tilesets()->at(id);
    emit added_tileset(current_map().value(), id, tileset);
  });
  connect(
      m_maps.get(), &map_manager::removed_tileset, [this](tileset_id tileset) {
        emit removed_tileset(current_map().value(), tileset);
      });
}

void model::undo()
{
  if (auto* document = current_document()) {
    document->undo();
    emit redraw();
  }
}

void model::redo()
{
  if (auto* document = current_document()) {
    document->redo();
    emit redraw();
  }
}

void model::resize_map(row_t nRows, col_t nCols)
{
  if (auto* document = current_document()) {
    document->resize(nRows, nCols);
    emit redraw();
  }
}

void model::add_row()
{
  if (auto* document = current_document()) {
    document->add_row();
    emit redraw();
  }
}

void model::add_col()
{
  if (auto* document = current_document()) {
    document->add_column();
    emit redraw();
  }
}

void model::remove_row()
{
  if (auto* document = current_document()) {
    document->remove_row();
    emit redraw();
  }
}

void model::remove_col()
{
  if (auto* document = current_document()) {
    document->remove_column();
    emit redraw();
  }
}

void model::select_layer(layer_id id)
{
  if (auto* document = current_document()) {
    document->select_layer(id);
    emit redraw();
  }
}

void model::increase_tile_size()
{
  if (auto* document = current_document()) {
    document->increase_tile_size();
    emit redraw();
  }
}

void model::decrease_tile_size()
{
  if (auto* document = current_document()) {
    document->decrease_tile_size();
    emit redraw();
  }
}

void model::reset_tile_size()
{
  if (auto* document = current_document()) {
    document->reset_tile_size();
    emit redraw();
  }
}

void model::ui_selected_map(map_id id)
{
  m_maps->select(id);
  emit switched_map(id);
}

void model::ui_removed_tileset(tileset_id id)
{
  m_maps->ui_removed_tileset(id);
  emit redraw();
}

}  // namespace tactile::core
