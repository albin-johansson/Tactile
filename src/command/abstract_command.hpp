#pragma once

#include <qstring.h>
#include <qundostack.h>

#include "map.hpp"
#include "types.hpp"

namespace tactile::cmd {

class abstract_command : public QUndoCommand
{
 protected:
  explicit abstract_command(const QString& text, not_null<core::map*> map);

  core::map* m_map;
};

}  // namespace tactile::cmd
