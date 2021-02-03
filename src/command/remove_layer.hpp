#pragma once

#include <QUndoCommand>

#include "layer.hpp"
#include "layer_id.hpp"
#include "remove_layer.hpp"
#include "smart_pointers.hpp"

namespace tactile {
namespace core {
class map_document;
}

namespace cmd {

class remove_layer final : public QUndoCommand
{
 public:
  remove_layer(core::map_document* document, layer_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return static_cast<int>(command_id::remove_layer);
  }

 private:
  core::map_document* m_document{};
  shared<core::layer> m_layer;
  layer_id m_id;
  int m_index{};
};

}  // namespace cmd
}  // namespace tactile
