#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "command_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class ChangePropertyType final : public QUndoCommand
{
 public:
  ChangePropertyType(not_null<core::IPropertyManager*> manager,
                     QString name,
                     core::PropertyType type1);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::ChangePropertyType);
  }

 private:
  core::IPropertyManager* mManager{};
  QString mName;
  core::PropertyType mType;
  maybe<core::Property> mPreviousProperty;
};

}  // namespace tactile::cmd
