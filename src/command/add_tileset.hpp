#pragma once

#include <QUndoCommand>
#include <memory>  // shared_ptr

#include "command_id.hpp"
#include "map_document.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"

namespace tactile::cmd {

class add_tileset final : public QUndoCommand
{
 public:
  add_tileset(core::map_document* document,
              std::shared_ptr<core::tileset> tileset,
              tileset_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(id::add_tileset);
  }

 private:
  core::map_document* m_document;
  std::shared_ptr<core::tileset> m_tileset;
  tileset_id m_id;
};

}  // namespace tactile::cmd
