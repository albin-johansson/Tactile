#pragma once

#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"

namespace Tactile {

class MoveLayerDownCmd final : public MapCommand
{
 public:
  MoveLayerDownCmd(NotNull<MapDocument*> document, layer_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::MoveLayerDown;
  }

 private:
  layer_id mId;
};

}  // namespace Tactile
