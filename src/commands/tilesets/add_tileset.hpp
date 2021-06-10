#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "map_document.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"

namespace tactile::cmd {

class AddTileset final : public QUndoCommand
{
 public:
  AddTileset(not_null<core::map_document*> document,
             shared<core::tileset> tileset,
             tileset_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::add_tileset);
  }

 private:
  core::map_document* mDocument{};
  shared<core::tileset> mTileset;
  tileset_id mId;
};

}  // namespace tactile::cmd
