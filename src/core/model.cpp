#include "model.hpp"

#include "map_document_manager.hpp"
#include "property_model.hpp"

namespace tactile::core {

model::model() : m_mapDocuments{new map_document_manager{this}}, m_tools{this}
{
  // clang-format off
  connect(m_mapDocuments, &map_document_manager::redraw,
          this,           &model::redraw);

  connect(m_mapDocuments, &map_document_manager::undo_state_updated,
          this,           &model::undo_state_updated);

  connect(m_mapDocuments, &map_document_manager::redo_state_updated,
          this,           &model::redo_state_updated);

  connect(m_mapDocuments, &map_document_manager::undo_text_updated,
          this,           &model::undo_text_updated);

  connect(m_mapDocuments, &map_document_manager::redo_text_updated,
          this,           &model::redo_text_updated);

  connect(m_mapDocuments, &map_document_manager::clean_changed,
          this,           &model::clean_changed);

  connect(m_mapDocuments, &map_document_manager::added_layer,
          this,           &model::added_layer);

  connect(m_mapDocuments, &map_document_manager::added_duplicated_layer,
          this,           &model::added_duplicated_layer);

  connect(m_mapDocuments, &map_document_manager::removed_layer,
          this,           &model::removed_layer);

  connect(m_mapDocuments, &map_document_manager::selected_layer,
          this,           &model::selected_layer);

  connect(m_mapDocuments, &map_document_manager::moved_layer_back,
          this,           &model::moved_layer_back);

  connect(m_mapDocuments, &map_document_manager::moved_layer_forward,
          this,           &model::moved_layer_forward);

  connect(m_mapDocuments, &map_document_manager::removed_tileset,
          this,           &model::removed_tileset);

  connect(m_mapDocuments, &map_document_manager::added_property,
          this,           &model::added_property);

  connect(m_mapDocuments, &map_document_manager::about_to_remove_property,
          this,           &model::about_to_remove_property);

  connect(m_mapDocuments, &map_document_manager::updated_property,
          this,           &model::updated_property);

  connect(m_mapDocuments, &map_document_manager::renamed_property,
          this,           &model::renamed_property);
  // clang-format on

  connect(m_mapDocuments,
          &map_document_manager::added_tileset,
          [this](const tileset_id id) {
            const auto& tileset = current_document()->tilesets()->at(id);
            emit added_tileset(current_map_id().value(), id, tileset);
          });
}

auto model::add_map() -> map_id
{
  return m_mapDocuments->add();
}

auto model::add_map(map_document* document) -> map_id
{
  return m_mapDocuments->add(document);
}

auto model::has_active_map() const noexcept -> bool
{
  return m_mapDocuments->has_active_map();
}

auto model::get_document(const map_id id) -> map_document*
{
  return m_mapDocuments->at(id);
}

auto model::current_map_id() const -> maybe<map_id>
{
  return m_mapDocuments->current_map_id();
}

auto model::current_document() -> map_document*
{
  return m_mapDocuments->current_document();
}

auto model::current_document() const -> const map_document*
{
  return m_mapDocuments->current_document();
}

void model::undo()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->undo();
}

void model::redo()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->redo();
}

void model::resize_map(const row_t nRows, const col_t nCols)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->resize(nRows, nCols);
}

void model::add_row()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->add_row();
}

void model::add_col()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->add_column();
}

void model::remove_row()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->remove_row();
}

void model::remove_col()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->remove_column();
}

void model::add_layer()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->add_tile_layer();
}

void model::remove_layer(const layer_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->remove_layer(id);
}

void model::select_layer(const layer_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->select_layer(id);
}

void model::select_tool(const tool_id id)
{
  m_tools.select(id);
}

void model::select_tileset(const tileset_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->select_tileset(id);
}

void model::set_tileset_selection(const tileset::selection& selection)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->set_tileset_selection(selection);
}

void model::increase_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->increase_tile_size();
}

void model::decrease_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->decrease_tile_size();
}

void model::reset_tile_size()
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->reset_tile_size();
}

void model::create_tileset(const QImage& image,
                           const QFileInfo& path,
                           const QString& name,
                           const tile_width tileWidth,
                           const tile_height tileHeight)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->add_tileset(image, path, name, tileWidth, tileHeight);
}

void model::remove_tileset(const tileset_id id)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->ui_remove_tileset(id);
}

void model::set_tileset_name(const tileset_id id, const QString& name)
{
  auto* document = current_document();
  Q_ASSERT(document);

  document->set_tileset_name(id, name);
}

void model::select_map(const map_id id)
{
  m_mapDocuments->select(id);

  auto* document = current_document();
  Q_ASSERT(document);

  emit switched_map(id,
                    *document,
                    document->property_model(),
                    std::make_shared<vm::layer_model>(document));
}

void model::close_map(const map_id id)
{
  m_mapDocuments->close(id);
}

void model::mouse_pressed(QMouseEvent* event, const QPointF& mapPosition)
{
  m_tools.pressed(event, mapPosition);
}

void model::mouse_moved(QMouseEvent* event, const QPointF& mapPosition)
{
  m_tools.moved(event, mapPosition);
}

void model::mouse_released(QMouseEvent* event, const QPointF& mapPosition)
{
  m_tools.released(event, mapPosition);
}

void model::mouse_entered(QEvent* event)
{
  m_tools.entered(event);
}

void model::mouse_exited(QEvent* event)
{
  m_tools.exited(event);
}

}  // namespace tactile::core
