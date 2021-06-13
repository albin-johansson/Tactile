#include "add_column.hpp"

#include "invoke_n.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

AddColumn::AddColumn(not_null<core::MapDocument*> document)
    : RepeatedCommand{QStringLiteral(u"Add Column")}
    , mDocument{document}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void AddColumn::undo()
{
  QUndoCommand::undo();

  InvokeN(Amount(), [this] { mDocument->Raw().RemoveColumn(); });

  emit mDocument->S_Redraw();
}

void AddColumn::redo()
{
  QUndoCommand::redo();

  InvokeN(Amount(), [this] { mDocument->Raw().AddColumn(empty); });

  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
