#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

class MoveLayerForward final : public QUndoCommand
{
 public:
  MoveLayerForward(not_null<core::MapDocument*> document, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::MoveLayerForward);
  }

 private:
  core::MapDocument* mDocument{};
  layer_id mId;
};

}  // namespace tactile::cmd
