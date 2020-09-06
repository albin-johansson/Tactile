#include "app.hpp"

#include <qdockwidget.h>
#include <qpainter.h>

#include "app_connections.hpp"
#include "model.hpp"
#include "resize_dialog.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "window.hpp"

namespace tactile {

using core::model;

app::app(int argc, char** argv) : QApplication{argc, argv}, m_model{new model{}}
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
  delete m_model;
}

void app::handle_resize_map()
{
  if (m_model->has_active_map()) {
    gui::resize_dialog::spawn(
        [this](int rows, int cols) { m_model->resize_map(rows, cols); });
  }
}

void app::handle_pan_up()
{
  if (const auto tileSize = m_model->tile_size(); tileSize) {
    m_window->handle_move_camera(0, *tileSize);
  }
}

void app::handle_pan_down()
{
  if (const auto tileSize = m_model->tile_size(); tileSize) {
    m_window->handle_move_camera(0, -(*tileSize));
  }
}

void app::handle_pan_right()
{
  if (const auto tileSize = m_model->tile_size(); tileSize) {
    m_window->handle_move_camera(-(*tileSize), 0);
  }
}

void app::handle_pan_left()
{
  if (const auto tileSize = m_model->tile_size(); tileSize) {
    m_window->handle_move_camera(*tileSize, 0);
  }
}

void app::handle_new_tileset()
{
  gui::tileset_dialog::spawn([this](const QImage& image,
                                    tile_width tileWidth,
                                    tile_height tileHeight,
                                    const QString& name) {
    if (const auto id = m_model->add_tileset(image, tileWidth, tileHeight);
        id) {
      m_window->handle_add_tileset(image, *id, tileWidth, tileHeight, name);
    }
  });
}

void app::tileset_selection_changed(core::position topLeft,
                                    core::position bottomRight)
{
  m_model->update_tileset_selection(topLeft, bottomRight);
}

void app::handle_new_map()
{
  const auto id = m_model->add_map();
  m_window->handle_new_map(m_model->get_map(id), id);
}

}  // namespace tactile
