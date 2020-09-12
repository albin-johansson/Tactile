#pragma once

#include "abstract_command.hpp"
#include "position.hpp"
#include "types.hpp"
#include "vector_map.hpp"

namespace tactile::cmd {

class erase_sequence final : public abstract_command
{
 public:
  erase_sequence(not_null<core::map*> map,
                 vector_map<core::position, tile_id>&& oldState);

  void undo() override;

  void redo() override;

 private:
  vector_map<core::position, tile_id> m_oldState;
  bool m_first{true};
};

}  // namespace tactile::cmd
