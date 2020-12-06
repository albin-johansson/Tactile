#include "window.hpp"

#include "about_dialog.hpp"
#include "layer_dock.hpp"
#include "map_editor.hpp"
#include "open_map_dialog.hpp"
#include "preferences.hpp"
#include "properties_dock.hpp"
#include "save_as_dialog.hpp"
#include "save_service.hpp"
#include "setting.hpp"
#include "settings_dialog.hpp"
#include "tileset_dock.hpp"
#include "tool_dock.hpp"
#include "ui_window.h"

namespace tactile::gui {

window::window(QWidget* parent) : QMainWindow{parent}, m_ui{new Ui::window{}}
{
  m_ui->setupUi(this);

  m_editor = new map_editor{this};
  m_toolDock = new tool_dock{this};
  m_layerDock = new layer_dock{this};
  m_tilesetDock = new tileset_dock{this};
  m_propertiesDock = new properties_dock{this};

  setCentralWidget(m_editor);
  addDockWidget(Qt::LeftDockWidgetArea, m_toolDock);
  addDockWidget(Qt::RightDockWidgetArea, m_tilesetDock);
  addDockWidget(Qt::RightDockWidgetArea, m_layerDock);
  addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);

  tabifyDockWidget(m_layerDock, m_propertiesDock);
  m_layerDock->raise();  // Layer dock is initially of greater interest

  init_mouse_tool_group();
  init_connections();

  showMaximized();

  restore_layout();
  enter_no_content_view();
}

window::~window() noexcept
{
  delete m_ui;
}

void window::init_mouse_tool_group()
{
  Q_ASSERT(!m_toolGroup);

  m_toolGroup = new QActionGroup{this};
  m_toolGroup->setExclusive(true);
  m_toolGroup->addAction(m_ui->actionStampTool);
  m_toolGroup->addAction(m_ui->actionBucketTool);
  m_toolGroup->addAction(m_ui->actionEraserTool);

  // clang-format off
  connect(m_toolDock, &tool_dock::enable_stamp, this, &window::stamp_enabled);
  connect(m_toolDock, &tool_dock::enable_bucket, this, &window::bucket_enabled);
  connect(m_toolDock, &tool_dock::enable_eraser, this, &window::eraser_enabled);
  // clang-format on
}

void window::init_connections()
{
  // clang-format off
  connect(m_ui->actionNewMap,     &QAction::triggered, this, &window::ui_new_map);
  connect(m_ui->actionAddTileset, &QAction::triggered, this, &window::ui_add_tileset);

  connect(m_toolDock,    &QDockWidget::visibilityChanged, m_ui->actionToolsVisibility, &QAction::setChecked);
  connect(m_tilesetDock, &QDockWidget::visibilityChanged, m_ui->actionTilesetsVisibility, &QAction::setChecked);
  connect(m_layerDock,   &QDockWidget::visibilityChanged, m_ui->actionLayersVisibility, &QAction::setChecked);

  connect(m_toolDock,       &tool_dock::closed, [] { prefs::graphics::tool_widget_visible().set(false); });
  connect(m_layerDock,      &tool_dock::closed, [] { prefs::graphics::layer_widget_visible().set(false); });
  connect(m_tilesetDock,    &tool_dock::closed, [] { prefs::graphics::tileset_widget_visible().set(false); });
  connect(m_propertiesDock, &tool_dock::closed, [] { prefs::graphics::properties_widget_visible().set(false); });

  connect(m_editor, &map_editor::ui_select_map,  this, &window::ui_select_map);
  connect(m_editor, &map_editor::ui_remove_map,  this, &window::handle_remove_map);
  connect(m_editor, &map_editor::increase_zoom,  this, &window::ui_increase_zoom);
  connect(m_editor, &map_editor::decrease_zoom,  this, &window::ui_decrease_zoom);
  connect(m_editor, &map_editor::mouse_pressed,  this, &window::mouse_pressed);
  connect(m_editor, &map_editor::mouse_moved,    this, &window::mouse_moved);
  connect(m_editor, &map_editor::mouse_released, this, &window::mouse_released);
  connect(m_editor, &map_editor::mouse_entered,  this, &window::mouse_entered);
  connect(m_editor, &map_editor::mouse_exited,   this, &window::mouse_exited);

  connect(m_tilesetDock, &tileset_dock::ui_add_tileset,           this, &window::ui_add_tileset);
  connect(m_tilesetDock, &tileset_dock::ui_select_tileset,        this, &window::ui_select_tileset);
  connect(m_tilesetDock, &tileset_dock::ui_remove_tileset,        this, &window::ui_remove_tileset);
  connect(m_tilesetDock, &tileset_dock::ui_rename_tileset,        this, &window::ui_rename_tileset);
  connect(m_tilesetDock, &tileset_dock::ui_set_tileset_selection, this, &window::ui_set_tileset_selection);

  connect(m_layerDock, &layer_dock::ui_add_layer,            this, &window::ui_add_layer);
  connect(m_layerDock, &layer_dock::ui_remove_layer,         this, &window::ui_remove_layer);
  connect(m_layerDock, &layer_dock::ui_select_layer,         this, &window::ui_select_layer);
  connect(m_layerDock, &layer_dock::ui_set_layer_visibility, this, &window::ui_set_layer_visibility);
  connect(m_layerDock, &layer_dock::ui_set_layer_opacity,    this, &window::ui_set_layer_opacity);
  connect(m_layerDock, &layer_dock::ui_set_layer_name,       this, &window::ui_set_layer_name);
  connect(m_layerDock, &layer_dock::ui_move_layer_up,        this, &window::ui_move_layer_up);
  connect(m_layerDock, &layer_dock::ui_move_layer_down,      this, &window::ui_move_layer_down);
  connect(m_layerDock, &layer_dock::ui_duplicate_layer,      this, &window::ui_duplicate_layer);
  // clang-format on
}

void window::restore_layout()
{
  prefs::window::last_layout_geometry().with([this](const QByteArray& geom) {
    restoreGeometry(geom);
  });

  prefs::window::last_layout_state().with([this](const QByteArray& state) {
    restoreState(state);
  });
}

void window::reset_dock_layout()
{
  removeDockWidget(m_toolDock);
  removeDockWidget(m_tilesetDock);
  removeDockWidget(m_layerDock);
  removeDockWidget(m_propertiesDock);

  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_toolDock);
  prefs::graphics::reset_tool_widget_visible();
  m_toolDock->setVisible(prefs::graphics::tool_widget_visible().value());

  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tilesetDock);
  prefs::graphics::reset_tileset_widget_visible();
  m_tilesetDock->setVisible(prefs::graphics::tileset_widget_visible().value());

  addDockWidget(Qt::RightDockWidgetArea, m_layerDock);
  prefs::graphics::reset_layer_widget_visible();
  m_layerDock->setVisible(prefs::graphics::layer_widget_visible().value());

  addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);
  prefs::graphics::reset_properties_widget_visible();
  m_propertiesDock->setVisible(
      prefs::graphics::properties_widget_visible().value());
  tabifyDockWidget(m_layerDock, m_propertiesDock);
}

void window::hide_all_docks()
{
  m_toolDock->hide();
  m_layerDock->hide();
  m_tilesetDock->hide();
  m_propertiesDock->hide();
}

void window::restore_dock_visibility()
{
  prefs::graphics::tool_widget_visible().with([this](const bool value) {
    m_toolDock->setVisible(value);
  });

  prefs::graphics::tileset_widget_visible().with([this](const bool value) {
    m_tilesetDock->setVisible(value);
  });

  prefs::graphics::layer_widget_visible().with([this](const bool value) {
    m_layerDock->setVisible(value);
  });

  prefs::graphics::properties_widget_visible().with([this](const bool value) {
    m_propertiesDock->setVisible(value);
  });
}

void window::center_viewport()
{
  m_editor->center_viewport();
}

void window::set_actions_enabled(const bool enabled)
{
  // File
  m_ui->actionCloseMap->setEnabled(enabled);
  m_ui->actionSave->setEnabled(enabled);
  m_ui->actionSaveAs->setEnabled(enabled);

  // Edit
  m_ui->actionAddCol->setEnabled(enabled);
  m_ui->actionAddRow->setEnabled(enabled);
  m_ui->actionRemoveRow->setEnabled(enabled);
  m_ui->actionRemoveCol->setEnabled(enabled);
  m_ui->actionResizeMap->setEnabled(enabled);
  m_ui->actionStampTool->setEnabled(enabled);
  m_ui->actionBucketTool->setEnabled(enabled);
  m_ui->actionEraserTool->setEnabled(enabled);
  m_ui->actionAddTileset->setEnabled(enabled);

  // View
  m_ui->actionResetLayout->setEnabled(enabled);
  m_ui->actionToolsVisibility->setEnabled(enabled);
  m_ui->actionTilesetsVisibility->setEnabled(enabled);
  m_ui->actionLayersVisibility->setEnabled(enabled);
  m_ui->actionCenterCamera->setEnabled(enabled);
  m_ui->actionToggleGrid->setEnabled(enabled);
  m_ui->actionZoomIn->setEnabled(enabled);
  m_ui->actionZoomOut->setEnabled(enabled);
  m_ui->actionResetZoom->setEnabled(enabled);
  m_ui->actionPanUp->setEnabled(enabled);
  m_ui->actionPanDown->setEnabled(enabled);
  m_ui->actionPanRight->setEnabled(enabled);
  m_ui->actionPanLeft->setEnabled(enabled);
}

auto window::in_editor_mode() const -> bool
{
  return m_editor->in_editor_mode();
}

void window::enter_no_content_view()
{
  m_editor->enable_startup_view();
  m_toolDock->disable_tools();

  set_actions_enabled(false);
  hide_all_docks();
}

void window::enter_content_view()
{
  m_editor->enable_editor_view();
  m_toolDock->enable_tools();

  set_actions_enabled(true);
  restore_dock_visibility();
}

void window::trigger_save_as()
{
  on_actionSaveAs_triggered();
}

void window::set_active_tab_name(const QString& name)
{
  m_editor->set_active_tab_name(name);
}

void window::force_redraw()
{
  m_editor->force_redraw();
}

void window::undo_state_updated(const bool canUndo)
{
  m_ui->actionUndo->setEnabled(canUndo);
}

void window::redo_state_updated(const bool canRedo)
{
  m_ui->actionRedo->setEnabled(canRedo);
}

void window::undo_text_updated(const QString& text)
{
  m_ui->actionUndo->setText(TACTILE_QSTRING(u"Undo ") + text);
}

void window::redo_text_updated(const QString& text)
{
  m_ui->actionRedo->setText(TACTILE_QSTRING(u"Redo ") + text);
}

void window::clean_changed(const bool clean)
{
  m_ui->actionSave->setDisabled(clean);
}

void window::added_tileset(const map_id map,
                           const tileset_id id,
                           const core::tileset& tileset)
{
  m_tilesetDock->added_tileset(map, id, tileset);
}

void window::removed_tileset(const tileset_id id)
{
  m_tilesetDock->removed_tileset(id);
}

void window::selected_layer(const layer_id id, const core::layer& layer)
{
  m_layerDock->selected_layer(id, layer);
}

void window::added_layer(const layer_id id, const core::layer& layer)
{
  m_layerDock->added_layer(id, layer);
}

void window::added_duplicated_layer(const layer_id id, const core::layer& layer)
{
  m_layerDock->added_duplicated_layer(id, layer);
}

void window::removed_layer(const layer_id id)
{
  m_layerDock->removed_layer(id);
}

void window::moved_layer_up(const layer_id id)
{
  m_layerDock->moved_layer_back(id);
}

void window::moved_layer_down(const layer_id id)
{
  m_layerDock->moved_layer_forward(id);
}

void window::switched_map(const map_id id, const core::map_document& document)
{
  m_tilesetDock->selected_map(id);
  m_layerDock->selected_map(id, document);
  m_propertiesDock->selected_map(document);
}

void window::enable_stamp_preview(const core::position& position)
{
  m_editor->enable_stamp_preview(position);
}

void window::disable_stamp_preview()
{
  m_editor->disable_stamp_preview();
}

void window::handle_move_camera(const int dx, const int dy)
{
  m_editor->move_map(dx, dy);
}

void window::handle_new_map(core::map_document* document,
                            const map_id id,
                            const QString& name)
{
  m_editor->add_map_tab(document, id, name);
  m_editor->select_tab(id);
  if (!in_editor_mode()) {
    enter_content_view();
    center_viewport();
  }

  m_ui->actionSave->setDisabled(document->is_clean());
}

void window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);
  prefs::window::last_layout_geometry().set(saveGeometry());
  prefs::window::last_layout_state().set(saveState());
}

void window::handle_remove_map(const map_id tabID)
{
  emit ui_close_map(tabID);

  // The tab isn't actually removed yet, this checks if there will be
  // no open tabs
  if (m_editor->tab_count() == 1) {
    enter_no_content_view();
  }
}

void window::handle_theme_changed()
{
  emit m_editor->theme_changed();
}

void window::stamp_enabled()
{
  m_ui->actionStampTool->setChecked(true);
  emit ui_selected_tool(tool_id::stamp);
}

void window::bucket_enabled()
{
  m_ui->actionBucketTool->setChecked(true);
  emit ui_selected_tool(tool_id::bucket);
}

void window::eraser_enabled()
{
  m_ui->actionEraserTool->setChecked(true);
  emit ui_selected_tool(tool_id::eraser);
}

void window::on_actionUndo_triggered()
{
  emit ui_undo();
}

void window::on_actionRedo_triggered()
{
  emit ui_redo();
}

void window::on_actionCloseMap_triggered()
{
  // TODO save current state of open map (exit saves?)
  const auto id = m_editor->active_tab_id().value();

  m_editor->close_tab(id);
  emit ui_close_map(id);

  if (m_editor->tab_count() == 0) {
    enter_no_content_view();
  }
}

void window::on_actionTilesetsVisibility_triggered()
{
  const auto visible = m_ui->actionTilesetsVisibility->isChecked();
  m_tilesetDock->setVisible(visible);
  prefs::graphics::tileset_widget_visible().set(visible);
}

void window::on_actionToolsVisibility_triggered()
{
  const auto visible = m_ui->actionToolsVisibility->isChecked();
  m_toolDock->setVisible(visible);
  prefs::graphics::tool_widget_visible().set(visible);
}

void window::on_actionLayersVisibility_triggered()
{
  const auto visible = m_ui->actionLayersVisibility->isChecked();
  m_layerDock->setVisible(visible);
  prefs::graphics::layer_widget_visible().set(visible);
}

void window::on_actionSave_triggered()
{
  emit ui_save();
}

void window::on_actionSaveAs_triggered()
{
  save_as_dialog::spawn(
      [this](const QString& path) {
        emit ui_save_as(path);
      },
      m_editor->active_tab_name().value_or(TACTILE_QSTRING(u"map")));
}

void window::on_actionOpenMap_triggered()
{
  open_map_dialog::spawn([this](const QString& path) {
    emit ui_open_map(path);
  });
}

void window::on_actionAddRow_triggered()
{
  emit ui_add_row();
}

void window::on_actionAddCol_triggered()
{
  emit ui_add_col();
}

void window::on_actionRemoveRow_triggered()
{
  emit ui_remove_row();
}

void window::on_actionRemoveCol_triggered()
{
  emit ui_remove_col();
}

void window::on_actionResizeMap_triggered()
{
  emit ui_resize_map();
}

void window::on_actionToggleGrid_triggered()
{
  auto grid = prefs::graphics::render_grid();
  grid.with([&, this](bool value) {
    grid.set(!value);
    force_redraw();
  });
}

void window::on_actionPanUp_triggered()
{
  emit ui_pan_up();
}

void window::on_actionPanDown_triggered()
{
  emit ui_pan_down();
}

void window::on_actionPanRight_triggered()
{
  emit ui_pan_right();
}

void window::on_actionPanLeft_triggered()
{
  emit ui_pan_left();
}

void window::on_actionZoomIn_triggered()
{
  emit ui_increase_zoom();
}

void window::on_actionZoomOut_triggered()
{
  emit ui_decrease_zoom();
}

void window::on_actionResetZoom_triggered()
{
  emit ui_reset_tile_size();
}

void window::on_actionCenterCamera_triggered()
{
  center_viewport();
}

void window::on_actionResetLayout_triggered()
{
  reset_dock_layout();
}

void window::on_actionStampTool_triggered()
{
  m_toolDock->stamp_enabled();
}

void window::on_actionBucketTool_triggered()
{
  m_toolDock->bucket_enabled();
}

void window::on_actionEraserTool_triggered()
{
  m_toolDock->eraser_enabled();
}

void window::on_actionSettings_triggered()  // NOLINT
{
  settings_dialog settings;

  // clang-format off
  connect(&settings, &settings_dialog::reload_theme, this, &window::handle_theme_changed);
  // clang-format on

  settings.exec();
}

void window::on_actionAboutQt_triggered()
{
  QApplication::aboutQt();
}

void window::on_actionExit_triggered()
{
  QApplication::exit();
}

void window::on_actionAbout_triggered()
{
  about_dialog about;
  about.exec();
}

}  // namespace tactile::gui
