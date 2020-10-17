#include "app_connections.hpp"

#include "model.hpp"
#include "window.hpp"

namespace tactile {

using core::model;
using gui::window;

namespace {

template <typename Sender, typename Signal, typename Receiver, typename Slot>
void connect(Sender&& sender, Signal&& signal, Receiver&& receiver, Slot&& slot)
{
  QObject::connect(sender, signal, receiver, slot);
}

}  // namespace

app_connections::app_connections(app* app)
    : m_model{app->model_ptr()},
      m_window{app->window_ptr()}
{
  using win = window;
  using mod = model;

  const auto model_to_window = [=](auto&& sender, auto&& receiver) {
    connect(m_model, sender, m_window, receiver);
  };

  const auto window_to_model = [=](auto&& sender, auto&& receiver) {
    connect(m_window, sender, m_model, receiver);
  };

  const auto window_to_app = [=](auto&& sender, auto&& receiver) {
    connect(m_window, sender, app, receiver);
  };

  // clang-format off

  model_to_window(&mod::redraw,                &win::handle_draw);
  model_to_window(&mod::enable_stamp_preview,  &win::enable_stamp_preview);
  model_to_window(&mod::disable_stamp_preview, &win::disable_stamp_preview);
  model_to_window(&mod::undo_state_updated,    &win::handle_undo_state_update);
  model_to_window(&mod::redo_state_updated,    &win::handle_redo_state_update);
  model_to_window(&mod::undo_text_updated,     &win::handle_undo_text_update);
  model_to_window(&mod::redo_text_updated,     &win::handle_redo_text_update);
  model_to_window(&mod::switched_map,          &win::switched_map);
  model_to_window(&mod::added_tileset,         &win::handle_add_tileset);
  model_to_window(&mod::removed_tileset,       &win::handle_removed_tileset);
  model_to_window(&mod::added_layer,           &win::handle_added_layer);
  model_to_window(&mod::removed_layer,         &win::handle_removed_layer);
  model_to_window(&mod::selected_layer,        &win::handle_selected_layer);

  window_to_model(&win::request_undo,               &mod::undo);
  window_to_model(&win::request_redo,               &mod::redo);
  window_to_model(&win::request_add_row,            &mod::add_row);
  window_to_model(&win::request_add_col,            &mod::add_col);
  window_to_model(&win::request_remove_row,         &mod::remove_row);
  window_to_model(&win::request_remove_col,         &mod::remove_col);
  window_to_model(&win::request_close_map,          &mod::close_map);
  window_to_model(&win::request_select_map,         &mod::ui_selected_map);
  window_to_model(&win::mouse_pressed,              &mod::mouse_pressed);
  window_to_model(&win::mouse_moved,                &mod::mouse_moved);
  window_to_model(&win::mouse_released,             &mod::mouse_released);
  window_to_model(&win::mouse_entered,              &mod::mouse_entered);
  window_to_model(&win::mouse_exited,               &mod::mouse_exited);
  window_to_model(&win::request_increase_tile_size, &mod::increase_tile_size);
  window_to_model(&win::request_decrease_tile_size, &mod::decrease_tile_size);
  window_to_model(&win::request_reset_tile_size,    &mod::reset_tile_size);
  window_to_model(&win::select_tool,                &mod::select_tool);
  window_to_model(&win::ui_removed_tileset,         &mod::ui_removed_tileset);
  window_to_model(&win::selected_tileset,           &mod::select_tileset);
  window_to_model(&win::ui_requested_new_layer,     &mod::add_layer);
  window_to_model(&win::ui_requested_remove_layer,  &mod::remove_layer);
  window_to_model(&win::ui_selected_layer,          &mod::select_layer);
  window_to_model(&win::ui_set_layer_visibility,    &mod::set_layer_visibility);

  window_to_app(&win::request_resize_map,        &app::handle_resize_map);
  window_to_app(&win::request_pan_up,            &app::handle_pan_up);
  window_to_app(&win::request_pan_down,          &app::handle_pan_down);
  window_to_app(&win::request_pan_right,         &app::handle_pan_right);
  window_to_app(&win::request_pan_left,          &app::handle_pan_left);
  window_to_app(&win::request_new_map,           &app::handle_new_map);
  window_to_app(&win::request_new_tileset,       &app::handle_new_tileset);
  window_to_app(&win::tileset_selection_changed, &app::tileset_selection_changed);
  window_to_app(&win::save_as,                   &app::save_as);
  window_to_app(&win::open_map,                  &app::open_map);

  // clang-format on
}

void app_connections::from_model()
{}

void app_connections::from_window()
{}

}  // namespace tactile
