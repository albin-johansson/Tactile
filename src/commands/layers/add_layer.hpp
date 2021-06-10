#pragma once

#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "forward_declare.hpp"
#include "layer.hpp"
#include "layer_id.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, MapDocument)

namespace tactile::cmd {

class AddLayer final : public QUndoCommand
{
 public:
  AddLayer(core::MapDocument* document,
           shared<core::ILayer> layer,
           layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::add_layer);
  }

 private:
  core::MapDocument* mDocument{};
  shared<core::ILayer> mLayer;
  layer_id mId;
};

}  // namespace tactile::cmd
