#include "app.hpp"

#include <QDockWidget>
#include <QFile>
#include <QIcon>
#include <QPainter>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "core.hpp"
#include "resize_dialog.hpp"
#include "settings_utils.hpp"
#include "tactile_window.hpp"
#include "tileset_dialog.hpp"
#include "tileset_info.hpp"

using namespace tactile::ui;
using namespace tactile::model;

namespace tactile {
namespace {

/**
 * Initialises the surface format used by the application.
 *
 * @since 0.1.0
 */
void init_surface_format() noexcept
{
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 2);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
}

/**
 * Initializes the settings to their default values, if they are absent.
 *
 * @since 0.1.0
 */
void validate_settings() noexcept
{
  set_if_absent("visuals-grid", true);
  set_if_absent("load-previous-layout-on-startup", true);
}

}  // namespace

app::app(int argc, char** argv) : QApplication{argc, argv}
{
  init_surface_format();

  setStyle(QStyleFactory::create("Fusion"));
  setApplicationVersion("0.1.0");
  setOrganizationName("albinjohansson");
  setApplicationName("Tactile");

  QIcon icon{":/resources/icons/tactile2_icon2.png"};
  setWindowIcon(icon);

  validate_settings();

  m_window = window::unique();
  m_core = core::unique();

  init_connections();
  load_style_sheet(":/resources/tactile_light.qss");

  // TODO do this to indicate what tool is active
  //  QPixmap pixmap{":/resources/icons/icons8/color/64/eraser.png"};
  //  QCursor c{pixmap};
  //  QApplication::setOverrideCursor(c);

  m_window->show();
}

void app::load_style_sheet(czstring styleSheet)
{
  if (!styleSheet) {
    return;
  }

  QFile file{styleSheet};
  if (file.open(QFile::ReadOnly)) {
    const QString style{file.readAll()};
    setStyleSheet(style);
  } else {
    qInfo("Failed to open stylesheet!");
  }
}

void app::init_connections() noexcept
{
  auto* window = m_window.get();
  auto* core = m_core.get();

  connect(core, &core::s_updated, window, &window::handle_trigger_redraw);

  window_to_core(&window::s_added_row, &core::handle_add_row);
  window_to_core(&window::s_added_row, &core::handle_add_row);
  window_to_core(&window::s_added_col, &core::handle_add_col);
  window_to_core(&window::s_removed_row, &core::handle_remove_row);
  window_to_core(&window::s_removed_col, &core::handle_remove_col);

  window_to_core(&window::s_new_map,
                 [core](int id) noexcept { core->handle_new_map(id); });

  window_to_core(&window::s_close_map, &core::handle_close_map);
  window_to_core(&window::s_select_map, &core::select_map);

  window_to_core(&window::s_redraw, &core::handle_draw);

  window_to_core(&window::s_increase_tile_size,
                 &core::handle_increase_tile_size);
  window_to_core(&window::s_decrease_tile_size,
                 &core::handle_decrease_tile_size);
  window_to_core(&window::s_reset_tile_size, &core::handle_reset_tile_size);

  window_to_core(&window::s_pan_up, [window, core] {
    if (const auto tileSize = core->tile_size(); tileSize) {
      window->handle_move_camera(0, *tileSize);
    }
  });

  window_to_core(&window::s_pan_right, [window, core] {
    if (const auto tileSize = core->tile_size(); tileSize) {
      window->handle_move_camera(-(*tileSize), 0);
    }
  });

  window_to_core(&window::s_pan_down, [window, core] {
    if (const auto tileSize = core->tile_size(); tileSize) {
      window->handle_move_camera(0, -(*tileSize));
    }
  });

  window_to_core(&window::s_pan_left, [window, core] {
    if (const auto tileSize = core->tile_size(); tileSize) {
      window->handle_move_camera(*tileSize, 0);
    }
  });

  window_to_this(&window::s_center_camera, [window, core] {
    const auto width = core->map_width();
    const auto height = core->map_height();
    if (width && height) {
      window->handle_center_camera(*width, *height);
    }
  });

  window_to_this(&window::s_new_tileset, [window, core] {
    TilesetDialog dialog;
    if (dialog.exec()) {
      const auto image = dialog.chosen_image();
      const auto tileWidth = dialog.chosen_width();
      const auto tileHeight = dialog.chosen_height();
      const auto imageName = dialog.image_name();
      if (image && tileWidth && tileHeight) {
        const auto id = core->add_tileset(image, *tileWidth, *tileHeight);
        if (id) {
          TilesetInfo info{image, *id, *tileWidth, *tileHeight};
          window->handle_add_tileset(info, imageName ? *imageName : "Untitled");
        }
      }
    }
  });

  window_to_this(&window::s_resize_map, [window, core] {
    ResizeDialog dialog;
    if (dialog.exec()) {
      core->set_rows(*dialog.chosen_height());
      core->set_cols(*dialog.chosen_width());
      window->handle_trigger_redraw();
    }
  });
}

}  // namespace tactile