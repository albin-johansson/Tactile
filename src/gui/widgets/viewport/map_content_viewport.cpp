#include "map_content_viewport.hpp"

#include <cmath>  // trunc

#include "core/events/close_map_event.hpp"
#include "core/events/mouse_drag_event.hpp"
#include "core/events/mouse_pressed_event.hpp"
#include "core/events/mouse_released_event.hpp"
#include "core/events/select_map_event.hpp"
#include "core/model.hpp"
#include "gui/canvas_info.hpp"
#include "gui/show_grid.hpp"
#include "gui/widgets/mouse_tracker.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "render_map.hpp"
#include "render_stamp_preview.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

inline constexpr auto tile_highlight_color = IM_COL32(0, 255, 0, 200);

inline bool center_viewport = false;
inline GridState state;

struct CursorInfo final
{
  MapPosition map_position;
  ImVec2 raw_position{};
  bool is_within_map{};
};

[[nodiscard]] auto GetCursorInfo(const ImVec2& mapOrigin,
                                 const float nRows,
                                 const float nCols) -> CursorInfo
{
  CursorInfo info;

  const auto mouse = ImGui::GetMousePos();
  const auto item = ImGui::GetItemRectMin();
  const auto diff = mouse - mapOrigin;

  const auto xRow = diff.y / state.grid_size.y;
  const auto xCol = diff.x / state.grid_size.x;
  const auto row = std::trunc(xRow);
  const auto col = std::trunc(xCol);

  info.is_within_map = xRow >= 0 && xCol >= 0 && row < nRows && col < nCols;

  if (info.is_within_map)
  {
    info.map_position = {row_t{static_cast<row_t::value_type>(row)},
                         col_t{static_cast<col_t::value_type>(col)}};
  }

  info.raw_position = {mapOrigin.x + (col * state.grid_size.x),
                       mapOrigin.y + (row * state.grid_size.y)};

  return info;
}

template <typename Event, typename T>
void CheckFor(const CursorInfo& cursor, entt::dispatcher& dispatcher, T&& query)
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);
  if (left || mid || right)
  {
    MouseInfo info;
    info.mouse_position_in_map = cursor.map_position;

    if (left)
    {
      info.button = cen::mouse_button::left;
    }
    else if (mid)
    {
      info.button = cen::mouse_button::middle;
    }
    else /*if (right)*/
    {
      info.button = cen::mouse_button::right;
    }

    dispatcher.enqueue<Event>(info);
  }
}

void ShowActiveMap(const Model& model,
                   const MapDocument& document,
                   entt::dispatcher& dispatcher)
{
  state.grid_size = {64, 64};

  auto* drawList = ImGui::GetWindowDrawList();
  const auto canvas = GetCanvasInfo();

  FillBackground(canvas);
  MouseTracker(canvas, state);

  drawList->PushClipRect(canvas.tl, canvas.br, true);

  const auto nRows = static_cast<float>(document.GetRowCount());
  const auto nCols = static_cast<float>(document.GetColumnCount());

  if (center_viewport)
  {
    const auto width = nCols * state.grid_size.x;
    const auto height = nRows * state.grid_size.y;

    state.scroll_offset.x = (canvas.size.x - width) / 2.0f;
    state.scroll_offset.y = (canvas.size.y - height) / 2.0f;

    center_viewport = false;
  }

  ShowGrid(state, canvas, IM_COL32(200, 200, 200, 15));

  const auto mapOrigin = canvas.tl + state.scroll_offset;
  RenderMap(document, drawList, mapOrigin, state.grid_size);

  const auto cursor = GetCursorInfo(mapOrigin, nRows, nCols);
  if (cursor.is_within_map)
  {
    drawList->AddRect(cursor.raw_position,
                      cursor.raw_position + state.grid_size,
                      tile_highlight_color,
                      0,
                      0,
                      2);

    if (ImGui::IsMouseHoveringRect(
            ImGui::GetWindowPos(),
            ImGui::GetWindowPos() + ImGui::GetWindowSize()))
    {
      CheckFor<MousePressedEvent>(cursor,
                                  dispatcher,
                                  [](const ImGuiMouseButton button) {
                                    return ImGui::IsMouseClicked(button);
                                  });

      CheckFor<MouseDragEvent>(cursor,
                               dispatcher,
                               [](const ImGuiMouseButton button) {
                                 return ImGui::IsMouseDragging(button);
                               });

      CheckFor<MouseReleasedEvent>(cursor,
                                   dispatcher,
                                   [](const ImGuiMouseButton button) {
                                     return ImGui::IsMouseReleased(button);
                                   });
    }

    const auto& tilesets = document.GetTilesets();
    const auto* tileset = tilesets.GetActiveTileset();
    if (model.GetActiveTool() == MouseToolType::Stamp && tileset &&
        tileset->GetSelection())
    {
      RenderStampPreview(mapOrigin,
                         state.grid_size,
                         document.GetMap(),
                         *tileset,
                         cursor.map_position);
    }
  }

  drawList->PopClipRect();
}

}  // namespace

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
          ShowActiveMap(model, *document, dispatcher);
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

void CenterMapContentViewport()
{
  center_viewport = true;
}

}  // namespace Tactile
