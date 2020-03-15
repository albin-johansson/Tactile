#include "tactile_window.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QSettings>
#include <QSpacerItem>
#include <QStandardPaths>
#include <iostream>

#include "about_dialog.h"
#include "central_editor_widget.h"
#include "settings_dialog.h"
#include "tile_size.h"
#include "ui_window.h"

namespace tactile {

TactileWindow::TactileWindow(QWidget* parent)
    : QMainWindow{parent}, m_ui{new Ui::MainWindow{}}
{
  m_ui->setupUi(this);

  // TODO add mouse tool and tile sheet widgets here

  m_centralWidget = new CentralEditorWidget{};
  setCentralWidget(m_centralWidget);

  init_connections();

  enable_startup_view();  // TODO option to reopen last tile map
}

TactileWindow::~TactileWindow() noexcept
{
  delete m_ui;
}

void TactileWindow::enable_startup_view() noexcept
{
  m_centralWidget->enable_startup_view();
}

void TactileWindow::enable_editor_view() noexcept
{
  m_centralWidget->enable_editor_view();
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

// void TactileWindow::redraw()
//{
//  QPainter painter{m_centralWidget->render_widget()};
//  painter.fillRect(0, 0, width(), height(), Qt::black);
//  painter.setViewport(m_centralWidget->current_viewport());
//  emit req_render(painter);
//}

void TactileWindow::center_camera(int mapWidth, int mapHeight)
{
  m_centralWidget->center_viewport(mapWidth, mapHeight);
  trigger_redraw();
}

void TactileWindow::trigger_redraw()
{
  m_centralWidget->trigger_redraw();
}

void TactileWindow::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);

  static bool first = true;
  if (first) {  // FIXME doesn't work correctly with stacked widgets
    first = false;
    emit req_center_camera();
  }
}

void TactileWindow::init_connections() noexcept
{
  using W = TactileWindow;

  const auto on_triggered = [this](QAction* action, auto fun) noexcept {
    connect(action, &QAction::triggered, this, fun);
  };

  on_triggered(m_ui->actionExit, [] { QApplication::exit(); });

  on_triggered(m_ui->actionNewMap, [this] {
    if (!in_editor_mode()) {
      enable_editor_view();
      emit req_center_camera();
    }

    // TODO...
  });

  on_triggered(m_ui->actionCloseMap, [this] {
    if (in_editor_mode()) {
      // TODO save current state of open map

      enable_startup_view();
    }
  });

  on_triggered(m_ui->actionAboutTactile, &W::display_about_dialog);
  on_triggered(m_ui->actionSettings, &W::display_settings_dialog);

  on_triggered(m_ui->actionAddTileSheet, [this] { emit req_new_tile_sheet(); });

  on_triggered(m_ui->actionAddRow, [this] {
    if (in_editor_mode()) {
      emit req_add_row();
    }
  });

  on_triggered(m_ui->actionAddColumn, [this] {
    if (in_editor_mode()) {
      emit req_add_col();
    }
  });

  on_triggered(m_ui->actionRemoveRow, [this] {
    if (in_editor_mode()) {
      emit req_remove_row();
    }
  });

  on_triggered(m_ui->actionRemoveColumn, [this] {
    if (in_editor_mode()) {
      emit req_remove_col();
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

  //  on_triggered(m_ui->actionPanUp, [this] {
  //    if (in_editor_mode()) {
  //      m_editorWidget->editor()->move_viewport(0, TileSize::get().size());
  //      trigger_redraw();
  //    }
  //  });

  //  on_triggered(m_ui->actionPanDown, [this] {
  //    if (in_editor_mode()) {
  //      m_editorWidget->editor()->move_viewport(0, -TileSize::get().size());
  //      trigger_redraw();
  //    }
  //  });
  //
  //  on_triggered(m_ui->actionPanRight, [this] {
  //    if (in_editor_mode()) {
  //      m_editorWidget->editor()->move_viewport(-TileSize::get().size(), 0);
  //      trigger_redraw();
  //    }
  //  });
  //
  //  on_triggered(m_ui->actionPanLeft, [this] {
  //    if (in_editor_mode()) {
  //      m_editorWidget->editor()->move_viewport(TileSize::get().size(), 0);
  //      trigger_redraw();
  //    }
  //  });

  on_triggered(m_ui->actionCenterCamera, [this] {
    if (in_editor_mode()) {
      emit req_center_camera();
    }
  });

  on_triggered(m_ui->actionResizeMap, [this] {
    if (in_editor_mode()) {
      emit req_resize_map();
    }
  });

  connect(
      m_centralWidget, &CentralEditorWidget::req_redraw, this, &W::req_render);
}

bool TactileWindow::in_editor_mode() const noexcept
{
  return m_centralWidget->in_editor_mode();
}

}  // namespace tactile
