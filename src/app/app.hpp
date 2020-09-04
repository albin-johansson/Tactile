#pragma once

#include <qapplication.h>

#include <memory>
#include <utility>

#include "core_model.hpp"
#include "fwd.hpp"
#include "types.hpp"
#include "window.hpp"

namespace tactile {

/**
 * @class app
 *
 * @brief Represents the tactile application.
 *
 * @see `QApplication`
 *
 * @since 0.1.0
 *
 * @headerfile app.hpp
 */
class app final : public QApplication
{
  Q_OBJECT

 public:
  friend class app_connections;

  /**
   * @param argc the amount of command-line arguments.
   * @param argv the array of command-line arguments.
   *
   * @since 0.1.0
   */
  app(int argc, char** argv);

  ~app() noexcept override;

 private:
  std::unique_ptr<gui::window> m_window;
  owner<core::core_model*> m_core;

  [[nodiscard]] auto window_ptr() noexcept -> gui::window*
  {
    return m_window.get();
  }

  [[nodiscard]] auto core_ptr() noexcept -> core::core_model*
  {
    return m_core;
  }

 private slots:
  void handle_resize_map();

  void handle_pan_up();

  void handle_pan_down();

  void handle_pan_right();

  void handle_pan_left();

  void handle_new_tileset();

  void handle_new_map();
};

}  // namespace tactile
