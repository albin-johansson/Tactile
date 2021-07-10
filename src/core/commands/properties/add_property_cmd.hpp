#pragma once

#include <string>  // string

#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/commands/properties/property_command.hpp"
#include "core/properties/property_type.hpp"

namespace Tactile {

class IPropertyContext;

class AddPropertyCmd final : public APropertyCommand
{
 public:
  AddPropertyCmd(NotNull<IPropertyContext*> context,
                 std::string name,
                 PropertyType type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddProperty;
  }

 private:
  std::string mName;
  PropertyType mType;
};

}  // namespace Tactile