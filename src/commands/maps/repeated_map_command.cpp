#include "repeated_map_command.hpp"

namespace tactile::cmd {

repeated_map_command::repeated_map_command(
    not_null<core::map_document*> document,
    const QString& name)
    : MapCommand{document, name}
{}

auto repeated_map_command::mergeWith(const QUndoCommand* other) -> bool
{
  if (id() == other->id())
  {
    if (auto* ptr = dynamic_cast<const repeated_map_command*>(other))
    {
      m_times += ptr->m_times;

      for (const auto& [layer, data] : ptr->LayerData())
      {
        auto& tiles = TileData(layer);
        for (const auto& [pos, tile] : data)
        {
          tiles.emplace(pos, tile);
        }
      }

      return true;
    }
  }
  return false;
}

}  // namespace tactile::cmd
