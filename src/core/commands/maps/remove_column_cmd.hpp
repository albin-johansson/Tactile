#pragma once

#include "core/commands/command_id.hpp"
#include "core/commands/maps/mergeable_map_command.hpp"

namespace Tactile {

class MapDocument;

class RemoveColumnCmd final : public MergeableMapCommand
{
 public:
  explicit RemoveColumnCmd(NotNull<MapDocument*> document);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveColumn;
  }
};

}  // namespace Tactile