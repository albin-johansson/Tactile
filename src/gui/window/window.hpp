#pragma once

#include <QActionGroup>  // QActionGroup
#include <QMainWindow>   // QMainWindow
#include <QString>       // QString
#include <QWidget>       // QWidget

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "map_id.hpp"
#include "not_null.hpp"
#include "position.hpp"
#include "property.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"
#include "tool_id.hpp"

TACTILE_FORWARD_DECLARE(Ui, window)
TACTILE_FORWARD_DECLARE(tactile::core, layer)
TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::core, property_manager)
TACTILE_FORWARD_DECLARE(tactile::gui, map_editor)
TACTILE_FORWARD_DECLARE(tactile::gui, tool_dock)
TACTILE_FORWARD_DECLARE(tactile::gui, layer_dock)
TACTILE_FORWARD_DECLARE(tactile::gui, tileset_dock)
TACTILE_FORWARD_DECLARE(tactile::gui, properties_dock)
TACTILE_FORWARD_DECLARE(tactile::gui, status_bar)

namespace tactile::gui {

/**
 * \class window
 *
 * \brief Represents the main window used in the application.
 *
 * \see `QMainWindow`
 *
 * \since 0.1.0
 *
 * \headerfile window.hpp
 */
class window final : public QMainWindow
{
  Q_OBJECT

 public:
  /**
   * \param parent a pointer to the parent widget, defaults to null.
   *
   * \since 0.1.0
   */
  explicit window(QWidget* parent = nullptr);

  ~window() noexcept override;

  /**
   * \brief Enables the view that is shown when no maps are active.
   *
   * \since 0.1.0
   */
  void enter_no_content_view();

  /**
   * \brief Enables the main editor view.
   *
   * \since 0.1.0
   */
  void enter_content_view();

  void trigger_save_as();

  void set_active_tab_name(const QString& name);

 signals:
  void ui_save();
  void ui_save_as(const QString& path);
  void ui_open_map(const QString& path);

  void ui_new_map();
  void ui_about_to_close_map(map_id id);

  void ui_undo();
  void ui_redo();

  void ui_add_row();
  void ui_add_col();

  void ui_remove_row();
  void ui_remove_col();

  void ui_selected_tool(tool_id tool);

  void ui_add_tileset();
  void ui_remove_tileset(tileset_id id);
  void ui_select_tileset(tileset_id id);
  void ui_rename_tileset(tileset_id id, const QString& name);
  void ui_set_tileset_selection(const core::tileset::selection& selection);

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

  void clean_changed(bool clean);

  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  void removed_tileset(tileset_id id);

  void selected_layer(layer_id id, const core::layer& layer);

  void added_layer(layer_id id, const core::layer& layer);

  void added_duplicated_layer(layer_id id, const core::layer& layer);

  void removed_layer(layer_id id);

  void added_property(const QString& name);

  void about_to_remove_property(const QString& name);

  void updated_property(const QString& name);

  void changed_property_type(const QString& name);

  void renamed_property(const QString& oldName, const QString& newName);

  void enable_stamp_preview(const core::position& position);

  void disable_stamp_preview();

  void handle_move_camera(int dx, int dy);

  void switched_map(map_id map, not_null<core::map_document*> document);

  void when_new_map_added(not_null<core::map_document*> document,
                          map_id id,
                          const QString& name = TACTILE_QSTRING(u"map"));

  void show_map_properties(not_null<core::property_manager*> manager);
  void show_layer_properties(not_null<core::property_manager*> manager);

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  unique<Ui::window> m_ui;
  map_editor* m_editor{};
  tool_dock* m_toolDock{};
  layer_dock* m_layerDock{};
  tileset_dock* m_tilesetDock{};
  properties_dock* m_propertiesDock{};
  status_bar* m_statusBar{};
  QActionGroup* m_toolGroup{};

  void init_mouse_tool_group();

  /**
   * \brief Initializes all of the connections related to the internal
   * components of the window.
   *
   * \since 0.1.0
   */
  void init_connections();

  void restore_layout();

  void reset_dock_layout();

  void hide_all_docks();

  void restore_dock_visibility();

  void center_viewport();

  void set_actions_enabled(bool enabled);

  /**
   * \brief Indicates whether or not the editor view is enabled.
   *
   * \return `true` if the editor view is enabled; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto in_editor_mode() const -> bool;

 private slots:
  void when_about_to_close_map(map_id id);

  void handle_theme_changed();

  void stamp_enabled();

  void bucket_enabled();

  void eraser_enabled();

  [[maybe_unused]] void on_actionUndo_triggered();

  [[maybe_unused]] void on_actionRedo_triggered();

  [[maybe_unused]] void on_actionCloseMap_triggered();

  [[maybe_unused]] void on_actionTilesetsVisibility_triggered();

  [[maybe_unused]] void on_actionToolsVisibility_triggered();

  [[maybe_unused]] void on_actionLayersVisibility_triggered();

  [[maybe_unused]] void on_actionPropertiesVisibility_triggered();

  [[maybe_unused]] void on_actionSave_triggered();

  [[maybe_unused]] void on_actionSaveAs_triggered();

  [[maybe_unused]] void on_actionOpenMap_triggered();

  [[maybe_unused]] void on_actionAddRow_triggered();

  [[maybe_unused]] void on_actionAddCol_triggered();

  [[maybe_unused]] void on_actionRemoveRow_triggered();

  [[maybe_unused]] void on_actionRemoveCol_triggered();

  [[maybe_unused]] void on_actionResizeMap_triggered();

  [[maybe_unused]] void on_actionToggleGrid_triggered();

  [[maybe_unused]] void on_actionPanUp_triggered();

  [[maybe_unused]] void on_actionPanDown_triggered();

  [[maybe_unused]] void on_actionPanRight_triggered();

  [[maybe_unused]] void on_actionPanLeft_triggered();

  [[maybe_unused]] void on_actionZoomIn_triggered();

  [[maybe_unused]] void on_actionZoomOut_triggered();

  [[maybe_unused]] void on_actionResetZoom_triggered();

  [[maybe_unused]] void on_actionCenterCamera_triggered();

  [[maybe_unused]] void on_actionResetLayout_triggered();

  [[maybe_unused]] void on_actionStampTool_triggered();

  [[maybe_unused]] void on_actionBucketTool_triggered();

  [[maybe_unused]] void on_actionEraserTool_triggered();

  [[maybe_unused]] void on_actionSettings_triggered();

  [[maybe_unused]] static void on_actionAboutQt_triggered();

  [[maybe_unused]] static void on_actionExit_triggered();

  [[maybe_unused]] static void on_actionAbout_triggered();
};

}  // namespace tactile::gui
