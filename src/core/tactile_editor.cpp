#include "tactile_editor.h"

#include "tile_map.h"

namespace tactile {

TactileEditor::TactileEditor()
{
  m_map = std::make_unique<TileMap>(5, 5);
}

TactileEditor::~TactileEditor() noexcept = default;

void TactileEditor::new_map() noexcept
{}

void TactileEditor::open_map(const char* fileName)
{
  // TODO parse Tactile or TMX formats
}

void TactileEditor::save_as(const char* fileName) const
{}

void TactileEditor::select_layer(int index) noexcept
{}

}  // namespace tactile
