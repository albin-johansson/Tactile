#include "duplicate_layer.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

duplicate_layer::duplicate_layer(not_null<core::map_document*> document,
                                 const layer_id id)
    : QUndoCommand{QTranslator::tr("Duplicate Layer")}
    , m_document{document}
    , m_id{id}
{
  if (!m_document) {
    throw tactile_error{"Null map document to duplicate layer command!"};
  }
}

void duplicate_layer::undo()
{
  QUndoCommand::undo();

  const auto id = m_newId.value();
  m_document->take_layer(id);
  emit m_document->removed_layer(id);

  // We need to tell the document that it can safely reuse the layer ID
  m_document->set_next_layer_id(id);

  m_newId.reset();
}

void duplicate_layer::redo()
{
  QUndoCommand::redo();

  m_newId = m_document->duplicate_layer_impl(m_id);
}

}  // namespace tactile::cmd
