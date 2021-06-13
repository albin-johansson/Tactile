#include "add_row.hpp"

#include "invoke_n.hpp"
#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

AddRow::AddRow(not_null<core::MapDocument*> document)
    : RepeatedCommand{QStringLiteral(u"Add Row")}
    , mDocument{document}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void AddRow::undo()
{
  QUndoCommand::undo();

  InvokeN(Amount(), [this] { mDocument->Raw().RemoveRow(); });

  emit mDocument->S_Redraw();
}

void AddRow::redo()
{
  QUndoCommand::redo();

  InvokeN(Amount(), [this] { mDocument->Raw().AddRow(empty); });

  emit mDocument->S_Redraw();
}

}  // namespace tactile::cmd
