#include "view_menu.hpp"

#include <imgui.h>

#include "core/events/center_viewport_event.hpp"
#include "core/model.hpp"
#include "gui/icons.hpp"
#include "gui/layout/dock_space.hpp"
#include "gui/widgets/layers/layer_dock.hpp"
#include "gui/widgets/properties/properties_dock.hpp"
#include "gui/widgets/tilesets/tileset_dock.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

void UpdateWidgetsSubmenu(const bool hasActiveMap, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMenu("Widgets", hasActiveMap))
  {
    if (ImGui::MenuItem("Reset layout"))
    {
      ResetLayout();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Properties", nullptr, Prefs::GetShowPropertiesDock()))
    {
      Prefs::SetShowPropertiesDock(!Prefs::GetShowPropertiesDock());
    }

    if (ImGui::MenuItem("Layers", nullptr, Prefs::GetShowLayerDock()))
    {
      Prefs::SetShowLayerDock(!Prefs::GetShowLayerDock());
    }

    if (ImGui::MenuItem("Tilesets", nullptr, Prefs::GetShowTilesetDock()))
    {
      Prefs::SetShowTilesetDock(!Prefs::GetShowTilesetDock());
    }

    ImGui::EndMenu();
  }
}

}  // namespace

void UpdateViewMenu(const Model& model, entt::dispatcher& dispatcher)
{
  const auto hasActiveMap = model.GetActiveMapId().has_value();

  if (ImGui::BeginMenu("View"))
  {
    UpdateWidgetsSubmenu(hasActiveMap, dispatcher);

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_CENTER " Center viewport",
                        "Ctrl+Space",
                        false,
                        hasActiveMap))
    {
      dispatcher.enqueue<CenterViewportEvent>();
    }

    bool showGrid = Prefs::GetShowGrid();
    if (ImGui::MenuItem(TAC_ICON_GRID " Toggle grid", "Ctrl+G", &showGrid))
    {
      Prefs::SetShowGrid(showGrid);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_ZOOM_IN " Increase zoom",
                        "Ctrl+Plus",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(TAC_ICON_ZOOM_OUT " Decrease zoom",
                        "Ctrl+Minus",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(TAC_ICON_RESET_ZOOM " Reset zoom",
                        nullptr,
                        false,
                        hasActiveMap))
    {}

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Pan up",
                        "Ctrl+Up",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Pan down",
                        "Ctrl+Down",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(TAC_ICON_MOVE_RIGHT " Pan right",
                        "Ctrl+Right",
                        false,
                        hasActiveMap))
    {}

    if (ImGui::MenuItem(TAC_ICON_MOVE_LEFT " Pan left",
                        "Ctrl+Left",
                        false,
                        hasActiveMap))
    {}

    ImGui::EndMenu();
  }
}

}  // namespace Tactile
