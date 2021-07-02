#pragma once

#include "core/commands/command_id.hpp"
#include "core/commands/maps/map_command.hpp"

namespace Tactile {

class MapDocument;

class AddColumnCmd final : public MapCommand
{
 public:
  explicit AddColumnCmd(NotNull<MapDocument*> document);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddColumn;
  }
};

}  // namespace Tactile
