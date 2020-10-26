#include "setup_app.hpp"

#include <QApplication>
#include <QIcon>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "app.hpp"
#include "preferences.hpp"
#include "theme.hpp"
#include "version.hpp"

namespace tactile {
namespace {

/**
 * \brief Initialises the surface format used by the application.
 *
 * \since 0.1.0
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

}  // namespace

void setup_app()
{
  init_surface_format();

  QGuiApplication::setWindowIcon(QIcon{":logo/windowIcon"});

  QApplication::setStyle(QStyleFactory::create("Fusion"));
  QApplication::setApplicationVersion(version());
  QApplication::setOrganizationName("albin-johansson");
  QApplication::setApplicationName("tactile");

  prefs::validate();

  if (const auto name = prefs::graphics::theme_name(); name) {
    theme::set_theme(*name);
  } else {
    theme::reset();
  }
}

}  // namespace tactile
