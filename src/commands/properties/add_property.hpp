#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand
#include <variant>       // variant, get_if, get

#include "command_id.hpp"
#include "not_null.hpp"
#include "property.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class AddProperty final : public QUndoCommand
{
 public:
  AddProperty(not_null<core::IPropertyManager*> manager,
              QString name,
              const core::Property& property);

  AddProperty(not_null<core::IPropertyManager*> manager,
              QString name,
              core::PropertyType type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(CommandId::add_property);
  }

 private:
  using data_type = std::variant<core::PropertyType, core::Property>;

  core::IPropertyManager* mManager{};
  data_type mData;
  QString mName;

  AddProperty(not_null<core::IPropertyManager*> manager,
              QString name,
              data_type data);
};

}  // namespace tactile::cmd
