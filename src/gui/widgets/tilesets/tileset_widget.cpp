#include "tileset_widget.hpp"

#include <imgui.h>

#include "core/events/remove_tileset_event.hpp"
#include "core/model.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/centered_button.hpp"
#include "gui/widgets/centered_text.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "gui/widgets/tilesets/tileset_content_widget.hpp"
#include "io/preferences.hpp"

namespace Tactile {

void UpdateTilesetWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowTilesetDock() || !model.GetActiveMapId())
  {
    return;
  }

  const auto* document = model.GetActiveDocument();
  const auto& tilesets = document->GetTilesets();

  bool isVisible = Prefs::GetShowTilesetDock();
  if (ImGui::Begin("Tilesets",
                   &isVisible,
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
  {
    if (tilesets.GetSize() != 0)
    {
      TilesetContentWidget(*document, dispatcher);
    }
    else
    {
      PrepareVerticalAlignmentCenter(2);

      CenteredText("No available tilesets!");
      if (CenteredButton(TAC_ICON_TILESET " Create tileset..."))
      {
        EnableTilesetDialog();
      }
    }
  }

  Prefs::SetShowTilesetDock(isVisible);
  ImGui::End();
}

}  // namespace Tactile
