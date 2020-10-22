#pragma once

#include <QActionGroup>
#include <QMainWindow>
#include <QString>
#include <QWidget>

#include "map_document.hpp"
#include "map_id.hpp"
#include "position.hpp"
#include "tool_id.hpp"

namespace Ui {
class window;
}

namespace tactile::gui {

class map_editor;
class tool_dock;
class layer_dock;
class tileset_dock;

/**
 * @class window
 *
 * @brief Represents the main window used in the application.
 *
 * @see `QMainWindow`
 *
 * @since 0.1.0
 *
 * @headerfile window.hpp
 */
class window final : public QMainWindow
{
  Q_OBJECT

 public:
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   *
   * @since 0.1.0
   */
  explicit window(QWidget* parent = nullptr);

  ~window() noexcept override;

  /**
   * @brief Enables the view that is shown when no maps are active.
   *
   * @since 0.1.0
   */
  void enter_no_content_view();

  /**
   * @brief Enables the main editor view.
   *
   * @since 0.1.0
   */
  void enter_content_view();

 signals:
  void ui_save_as(const QString& path);
  void ui_open_map(const QString& path);

  void ui_new_map();
  void ui_close_map(map_id id);

  void ui_undo();
  void ui_redo();

  void ui_add_row();
  void ui_add_col();

  void ui_remove_row();
  void ui_remove_col();

  void ui_selected_tool(tool_id tool);

  void ui_requested_new_layer();
  void ui_requested_remove_layer();
  void ui_selected_layer(layer_id id);
  void ui_set_layer_visibility(layer_id id, bool visible);
  void ui_set_layer_opacity(layer_id id, double opacity);
  void ui_set_layer_name(layer_id id, const QString& name);
  void ui_move_layer_down(layer_id id);
  void ui_move_layer_up(layer_id id);
  void ui_duplicate_layer(layer_id id);

  void ui_new_tileset();
  void ui_removed_tileset(tileset_id id);
  void ui_selected_tileset(tileset_id id);
  void ui_tileset_selection_changed(core::position topLeft,
                                    core::position bottomRight);

  void ui_resize_map();

  void ui_increase_zoom();
  void ui_decrease_zoom();
  void ui_reset_tile_size();

  void ui_pan_right();
  void ui_pan_down();
  void ui_pan_left();
  void ui_pan_up();

  void ui_select_map(map_id id);

  void mouse_pressed(QMouseEvent* event, QPointF mapPosition);
  void mouse_moved(QMouseEvent* event, QPointF mapPosition);
  void mouse_released(QMouseEvent* event, QPointF mapPosition);
  void mouse_entered(QEvent* event);
  void mouse_exited(QEvent* event);

 public slots:
  void force_redraw();

  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  void removed_tileset(tileset_id id);

  void selected_layer(layer_id id, const core::layer& layer);

  void added_layer(layer_id id, const core::layer& layer);

  void added_duplicated_layer(layer_id id, const core::layer& layer);

  void removed_layer(layer_id id);

  void moved_layer_up(layer_id id);

  void moved_layer_down(layer_id id);

  void switched_map(map_id map, const core::map_document& document);

  void enable_stamp_preview(const core::position& position);

  void disable_stamp_preview();

  void handle_move_camera(int dx, int dy);

  void handle_new_map(core::map_document* map, map_id id);

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  Ui::window* m_ui{};
  map_editor* m_editor{};
  tool_dock* m_toolDock{};
  layer_dock* m_layerDock{};
  tileset_dock* m_tilesetDock{};
  QActionGroup* m_toolGroup{};

  void init_mouse_tool_group();

  /**
   * @brief Initializes all of the connections related to the internal
   * components of the window.
   *
   * @since 0.1.0
   */
  void init_connections();

  void restore_layout();

  void reset_dock_layout();

  void hide_all_docks();

  void show_all_docks();

  void center_viewport();

  void set_actions_enabled(bool enabled);

  /**
   * @brief Indicates whether or not the editor view is enabled.
   *
   * @return `true` if the editor view is enabled; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto in_editor_mode() const -> bool;

 private slots:
  void handle_remove_map(map_id tabID);

  void handle_theme_changed();

  void stamp_enabled();

  void bucket_enabled();

  void eraser_enabled();

  void on_actionUndo_triggered();

  void on_actionRedo_triggered();

  void on_actionCloseMap_triggered();

  void on_actionTilesetsVisibility_triggered();

  void on_actionToolsVisibility_triggered();

  void on_actionLayersVisibility_triggered();

  void on_actionSave_triggered();

  void on_actionSaveAs_triggered();

  void on_actionOpenMap_triggered();

  void on_actionRename_triggered();

  void on_actionAddRow_triggered();

  void on_actionAddCol_triggered();

  void on_actionRemoveRow_triggered();

  void on_actionRemoveCol_triggered();

  void on_actionResizeMap_triggered();

  void on_actionToggleGrid_triggered();

  void on_actionPanUp_triggered();

  void on_actionPanDown_triggered();

  void on_actionPanRight_triggered();

  void on_actionPanLeft_triggered();

  void on_actionZoomIn_triggered();

  void on_actionZoomOut_triggered();

  void on_actionResetZoom_triggered();

  void on_actionCenterCamera_triggered();

  void on_actionResetLayout_triggered();

  void on_actionStampTool_triggered();

  void on_actionBucketTool_triggered();

  void on_actionEraserTool_triggered();

  void on_actionSettings_triggered();

  static void on_actionAboutQt_triggered();

  static void on_actionExit_triggered();

  static void on_actionAbout_triggered();
};

}  // namespace tactile::gui
