#include "remove_col.hpp"

namespace tactile::cmd {

remove_col::remove_col(not_null<core::map*> map)
    : abstract_command{QStringLiteral(u"Remove Column"), map}
{}

void remove_col::undo()
{
  QUndoCommand::undo();

  m_map->add_col();
}

void remove_col::redo()
{
  QUndoCommand::redo();

  m_map->remove_col();
}

}  // namespace tactile::cmd
