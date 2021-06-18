#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "fwd.hpp"
#include "maybe.hpp"
#include "property.hpp"
#include "to_underlying.hpp"

namespace tactile::cmd {

class RemoveProperty final : public QUndoCommand
{
 public:
  explicit RemoveProperty(core::IPropertyManager* manager, QString name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return ToUnderlying(CommandId::RemoveProperty);
  }

 private:
  core::IPropertyManager* mManager{};
  QString mName;
  Maybe<core::Property> mProperty;
};

}  // namespace tactile::cmd
