#pragma once

#include "aliases/ints.hpp"
#include "aliases/layer_id.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"
#include "core/map/layers/layer.hpp"

namespace Tactile {

class RemoveLayerCmd final : public MapCommand
{
 public:
  RemoveLayerCmd(NotNull<MapDocument*> document, layer_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveLayer;
  }

 private:
  SharedLayer mLayer;
  layer_id mId;
  usize mIndex{};
  bool mWasLayerActive{};
};

}  // namespace Tactile