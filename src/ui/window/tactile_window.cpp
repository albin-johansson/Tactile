#include "tactile_window.h"

#include <QActionGroup>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QSettings>
#include <QSpacerItem>
#include <QStandardPaths>

#include "about_dialog.h"
#include "central_editor_widget.h"
#include "mouse_tool_widget.h"
#include "settings_dialog.h"
#include "settings_utils.h"
#include "tile_sheet_widget.h"
#include "tile_size.h"
#include "ui_window.h"
#include "widget_size_policy.h"

namespace tactile {
namespace {

[[nodiscard]] UniquePtr<QDockWidget> create_mouse_tool_dock(
    QWidget* widget) noexcept
{
  auto dock = std::make_unique<QDockWidget>();
  dock->setObjectName("mouseToolDock");
  dock->setVisible(false);
  set_size_policy(dock.get(), QSizePolicy::Minimum, QSizePolicy::Expanding);
  dock->setWidget(widget);
  return dock;
}

[[nodiscard]] UniquePtr<QDockWidget> create_tile_sheet_dock(
    QWidget* widget) noexcept
{
  auto dock = std::make_unique<QDockWidget>();
  dock->setObjectName("tileSheetDock");
  dock->setVisible(false);
  set_size_policy(dock.get(), QSizePolicy::Expanding);
  dock->setWidget(widget);
  return dock;
}

}  // namespace

TactileWindow::TactileWindow(QWidget* parent)
    : QMainWindow{parent}, m_ui{new Ui::MainWindow{}}
{
  m_ui->setupUi(this);

  // TODO add mini-map and tile sheet widgets

  m_centralWidget = new CentralEditorWidget{};
  m_mouseToolWidget = new MouseToolWidget{};
  m_tileSheetWidget = new TileSheetWidget{};

  setCentralWidget(m_centralWidget);

  m_mouseToolDock = create_mouse_tool_dock(m_mouseToolWidget);
  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_mouseToolDock.get());

  m_tileSheetDock = create_tile_sheet_dock(m_tileSheetWidget);
  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tileSheetDock.get());

  init_connections();
  init_layout();
  enable_startup_view();  // TODO option to reopen last tile map
}

TactileWindow::~TactileWindow() noexcept
{
  delete m_ui;
}

void TactileWindow::enable_startup_view() noexcept
{
  m_centralWidget->enable_startup_view();
  m_mouseToolWidget->disable_tools();
  hide_all_docks();
}

void TactileWindow::enable_editor_view() noexcept
{
  m_centralWidget->enable_editor_view();
  m_mouseToolWidget->enable_tools();  // FIXME only enable relevant tools
}

void TactileWindow::display_about_dialog() noexcept
{
  AboutDialog about;
  about.exec();
}

void TactileWindow::display_settings_dialog() noexcept
{
  SettingsDialog settings;
  settings.exec();
}

void TactileWindow::center_camera(int mapWidth, int mapHeight)
{
  m_centralWidget->center_viewport(mapWidth, mapHeight);
  trigger_redraw();
}

void TactileWindow::trigger_redraw()
{
  m_centralWidget->trigger_redraw();
}

void TactileWindow::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);

  QSettings settings;
  settings.setValue("last-open-layout-state", saveState());
  settings.setValue("last-open-layout-geometry", saveGeometry());
}

void TactileWindow::init_connections() noexcept
{
  using W = TactileWindow;

  const auto on_triggered = [this](QAction* action, auto fun) noexcept {
    connect(action, &QAction::triggered, this, fun);
  };

  connect(m_centralWidget,
          &CentralEditorWidget::ce_removed_tab,
          this,
          [this](int id) noexcept {
            emit tw_close_map(id);

            // FIXME borderline hack, the tab isn't actually removed yet so 1
            //  is the same as checking if there are not open tabs.
            if (m_centralWidget->open_tabs() == 1) {
              enable_startup_view();
            }
          });

  on_triggered(m_ui->actionExit, [] { QApplication::exit(); });

  on_triggered(m_ui->actionNewMap, [this]() noexcept {
    const auto id = m_centralWidget->add_new_map_tab("map");

    emit tw_new_map(id);

    if (!in_editor_mode()) {
      enable_editor_view();
      show_all_docks();  // TODO just reopen docks that were visible
      emit tw_center_camera();
    }

    // TODO...
  });

  on_triggered(m_ui->actionCloseMap, [this]() noexcept {
    if (in_editor_mode()) {
      // TODO save current state of open map

      const auto id = m_centralWidget->active_tab_id();
      if (id) {
        m_centralWidget->close_tab(*id);
        emit tw_close_map(*id);

        if (m_centralWidget->open_tabs() == 0) {
          enable_startup_view();
        }
      } else {
        qWarning("Failed to close map, no active tab ID!");
      }
    }
  });

  on_triggered(m_ui->actionAboutTactile, &W::display_about_dialog);
  on_triggered(m_ui->actionSettings, &W::display_settings_dialog);

  on_triggered(m_ui->actionAddTileSheet,
               [this]() noexcept { emit tw_new_tile_sheet(); });

  on_triggered(m_ui->actionAddRow, [this]() noexcept {
    if (in_editor_mode()) {
      emit tw_added_row();
    }
  });

  on_triggered(m_ui->actionAddColumn, [this]() noexcept {
    if (in_editor_mode()) {
      emit tw_added_col();
    }
  });

  on_triggered(m_ui->actionRemoveRow, [this]() noexcept {
    if (in_editor_mode()) {
      emit tw_removed_row();
    }
  });

  on_triggered(m_ui->actionRemoveColumn, [this]() noexcept {
    if (in_editor_mode()) {
      emit tw_removed_col();
    }
  });

  on_triggered(m_ui->actionToggleGrid, [this] {
    if (in_editor_mode()) {
      QSettings settings;
      const auto prev = settings.value("visuals-grid").toBool();
      settings.setValue("visuals-grid", !prev);
      trigger_redraw();
    }
  });

  on_triggered(m_ui->actionZoomIn, [this] {
    if (in_editor_mode()) {
      TileSize::get().increase();
      trigger_redraw();
    }
  });

  on_triggered(m_ui->actionZoomOut, [this] {
    if (in_editor_mode()) {
      TileSize::get().decrease();
      trigger_redraw();
    }
  });

  on_triggered(m_ui->actionResetZoom, [this] {
    if (in_editor_mode()) {
      TileSize::get().reset();
      trigger_redraw();
    }
  });

  on_triggered(m_ui->actionPanUp, [this] {
    if (in_editor_mode()) {
      m_centralWidget->move_viewport(0, TileSize::get().size());
      trigger_redraw();
    }
  });

  on_triggered(m_ui->actionPanDown, [this] {
    if (in_editor_mode()) {
      m_centralWidget->move_viewport(0, -TileSize::get().size());
      trigger_redraw();
    }
  });

  on_triggered(m_ui->actionPanRight, [this] {
    if (in_editor_mode()) {
      m_centralWidget->move_viewport(-TileSize::get().size(), 0);
      trigger_redraw();
    }
  });

  on_triggered(m_ui->actionPanLeft, [this] {
    if (in_editor_mode()) {
      m_centralWidget->move_viewport(TileSize::get().size(), 0);
      trigger_redraw();
    }
  });

  on_triggered(m_ui->actionCenterCamera, [this] {
    if (in_editor_mode()) {
      emit tw_center_camera();
    }
  });

  on_triggered(m_ui->actionResizeMap, [this] {
    if (in_editor_mode()) {
      emit tw_resize_map();
    }
  });

  on_triggered(m_ui->actionMouseTools, [this] {
    m_mouseToolDock->setVisible(m_ui->actionMouseTools->isChecked());
  });

  on_triggered(m_ui->actionResetLayout, [this] { reset_dock_layout(); });

  {
    m_mouseToolGroup = std::make_unique<QActionGroup>(this);
    m_mouseToolGroup->setExclusive(true);
    m_mouseToolGroup->addAction(m_ui->actionStampTool);
    m_mouseToolGroup->addAction(m_ui->actionBucketTool);
    m_mouseToolGroup->addAction(m_ui->actionFindSameTool);
    m_mouseToolGroup->addAction(m_ui->actionEraserTool);
    m_mouseToolGroup->addAction(m_ui->actionRectangleTool);

    connect(m_mouseToolWidget, &MouseToolWidget::stamp_enabled, this, [this] {
      m_ui->actionStampTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MouseToolWidget::bucket_enabled, this, [this] {
      m_ui->actionBucketTool->setChecked(true);
    });

    connect(m_mouseToolWidget, &MouseToolWidget::eraser_enabled, this, [this] {
      m_ui->actionEraserTool->setChecked(true);
    });

    connect(
        m_mouseToolWidget, &MouseToolWidget::rectangle_enabled, this, [this] {
          m_ui->actionRectangleTool->setChecked(true);
        });

    connect(
        m_mouseToolWidget, &MouseToolWidget::find_same_enabled, this, [this] {
          m_ui->actionFindSameTool->setChecked(true);
        });

    on_triggered(m_ui->actionStampTool,
                 [this] { m_mouseToolWidget->enable_stamp(); });

    on_triggered(m_ui->actionBucketTool,
                 [this] { m_mouseToolWidget->enable_bucket(); });

    on_triggered(m_ui->actionEraserTool,
                 [this] { m_mouseToolWidget->enable_eraser(); });

    on_triggered(m_ui->actionRectangleTool,
                 [this] { m_mouseToolWidget->enable_rectangle(); });

    on_triggered(m_ui->actionFindSameTool,
                 [this] { m_mouseToolWidget->enable_find_same(); });
  }

  connect(m_mouseToolDock.get(),
          &QDockWidget::visibilityChanged,
          m_ui->actionMouseTools,
          &QAction::setChecked);

  connect(
      m_centralWidget, &CentralEditorWidget::req_redraw, this, &W::tw_render);
}

void TactileWindow::init_layout() noexcept
{
  const auto loadPrevious = settings_bool("load-previous-layout-on-startup");
  if (loadPrevious && loadPrevious.value()) {
    const auto state = settings_byte_array("last-open-layout-state");
    if (state) {
      restoreState(*state);
    }

    const auto geometry = settings_byte_array("last-open-layout-geometry");
    if (geometry) {
      restoreGeometry(*geometry);
    }
  }
}

void TactileWindow::reset_dock_layout() noexcept
{
  removeDockWidget(m_mouseToolDock.get());
  removeDockWidget(m_tileSheetDock.get());

  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_mouseToolDock.get());
  m_mouseToolDock->show();

  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tileSheetDock.get());
  m_tileSheetDock->show();
}

void TactileWindow::hide_all_docks() noexcept
{
  m_mouseToolDock->close();
  m_tileSheetDock->close();
}

void TactileWindow::show_all_docks() noexcept
{
  m_mouseToolDock->show();
  m_tileSheetDock->show();
}

bool TactileWindow::in_editor_mode() const noexcept
{
  return m_centralWidget->in_editor_mode();
}

}  // namespace tactile
