#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "fwd.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class SelectLayer final : public QUndoCommand
{
 public:
  SelectLayer(NotNull<core::MapDocument*> document, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::SelectLayer);
  }

 private:
  core::MapDocument* mDocument{};
  layer_id mId;
  Maybe<layer_id> mPrevious;
};

}  // namespace tactile::cmd
