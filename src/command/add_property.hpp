#pragma once

#include <QString>       // QString
#include <QUndoCommand>  // QUndoCommand

#include "property.hpp"
#include "property_manager.hpp"

namespace tactile::cmd {

class add_property final : public QUndoCommand
{
 public:
  explicit add_property(core::property_manager* manager,
                        QString name,
                        core::property::type type);

  void undo() override;

  void redo() override;

 private:
  core::property_manager* m_manager{};
  core::property::type m_type;
  QString m_name;
};

}  // namespace tactile::cmd
