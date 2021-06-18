#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "fwd.hpp"
#include "layer_id.hpp"
#include "map_position.hpp"
#include "not_null.hpp"
#include "tile_id.hpp"
#include "to_underlying.hpp"
#include "vector_map.hpp"

namespace tactile::cmd {

class EraseSequence final : public QUndoCommand
{
 public:
  EraseSequence(NotNull<core::MapDocument*> document,
                vector_map<core::MapPosition, tile_id>&& oldState);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::Eraser);
  }

 private:
  core::MapDocument* mDocument{};
  vector_map<core::MapPosition, tile_id> mOldState;
  layer_id mLayer;
  bool mFirst{true};
};

}  // namespace tactile::cmd
