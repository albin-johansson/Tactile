#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::cmd {

class DuplicateLayer final : public QUndoCommand
{
 public:
  DuplicateLayer(not_null<core::map_document*> document, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::duplicate_layer);
  }

 private:
  core::map_document* mDocument{};
  layer_id mId;
  maybe<layer_id> mNewId;
};

}  // namespace tactile::cmd