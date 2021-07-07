#include "map_content_viewport.hpp"

#include <imgui.h>

#include "core/events/maps/close_map_event.hpp"
#include "core/events/maps/select_map_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/viewport/map_view.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {

void MapContentViewport(const Model& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginTabBar("MapViewportTabBar", ImGuiTabBarFlags_Reorderable))
  {
    for (const auto& [id, document] : model)
    {
      const ScopeID uid{id};

      bool opened = true;
      const auto isActive = model.GetActiveMapId() == id;
      if (ImGui::BeginTabItem(document->GetName().data(),
                              &opened,
                              isActive ? ImGuiTabItemFlags_SetSelected : 0))
      {
        if (isActive)
        {
          MapView(model, *document, dispatcher);
        }

        ImGui::EndTabItem();
      }

      if (!opened)
      {
        dispatcher.enqueue<CloseMapEvent>(id);
      }
      else if (ImGui::IsItemActivated())
      {
        dispatcher.enqueue<SelectMapEvent>(id);
      }
    }

    ImGui::EndTabBar();
  }
}

}  // namespace Tactile
