#include "app.hpp"

#include <QDockWidget>
#include <QPainter>

#include "app_connections.hpp"
#include "core_model.hpp"
#include "resize_dialog.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "tileset_info.hpp"
#include "window.hpp"

namespace tactile {

using model::core_model;
using model::tileset_info;

app::app(int argc, char** argv)
    : QApplication{argc, argv}, m_core{new core_model{}}
{
  setup_app();

  m_window = std::make_unique<gui::window>();

  app_connections{*this};

  // do this to indicate what tool is active
  //  QPixmap pixmap{":/resources/icons/icons8/color/64/eraser.png"};
  //  QCursor c{pixmap};
  //  QApplication::setOverrideCursor(c);

  m_window->show();
}

app::~app() noexcept
{
  delete m_core;
}

void app::handle_resize_map()
{
  if (m_core->has_active_map()) {
    gui::resize_dialog dialog;

    if (dialog.exec()) {
      const auto rows = *dialog.chosen_height();
      const auto cols = *dialog.chosen_width();
      m_core->resize_map(rows, cols);
    }
  }
}

void app::handle_pan_up()
{
  if (const auto tileSize = m_core->tile_size(); tileSize) {
    m_window->handle_move_camera(0, *tileSize);
  }
}

void app::handle_pan_down()
{
  if (const auto tileSize = m_core->tile_size(); tileSize) {
    m_window->handle_move_camera(0, -(*tileSize));
  }
}

void app::handle_pan_right()
{
  if (const auto tileSize = m_core->tile_size(); tileSize) {
    m_window->handle_move_camera(-(*tileSize), 0);
  }
}

void app::handle_pan_left()
{
  if (const auto tileSize = m_core->tile_size(); tileSize) {
    m_window->handle_move_camera(*tileSize, 0);
  }
}

void app::handle_center_camera()
{
  const auto width = m_core->map_width();
  const auto height = m_core->map_height();
  if (width && height) {
    m_window->handle_center_camera(*width, *height);
  }
}

void app::handle_new_tileset()
{
  gui::tileset_dialog dialog;
  if (dialog.exec()) {
    const auto& image = dialog.chosen_image();
    const auto tileWidth = dialog.chosen_width();
    const auto tileHeight = dialog.chosen_height();
    const auto imageName = dialog.image_name();

    if (!image.isNull() && tileWidth && tileHeight) {
      const auto id = m_core->add_tileset(image, *tileWidth, *tileHeight);
      if (id) {
        tileset_info info{image, *id, *tileWidth, *tileHeight};
        m_window->handle_add_tileset(info, imageName ? *imageName : "Untitled");
      }
    }
  }
}

void app::handle_new_map()
{
  const auto id = m_core->add_map();
  m_window->handle_new_map(m_core->get_map(id), id);
}

}  // namespace tactile
