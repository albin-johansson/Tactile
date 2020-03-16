#include "tactile_application.h"

#include <QFile>
#include <QIcon>
#include <QPainter>
#include <QSettings>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include "resize_dialog.h"
#include "tactile_editor.h"
#include "tactile_window.h"
#include "tile_sheet_file_dialog.h"
#include "tile_size.h"

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
  QSettings settings;

  const auto set_if_absent = [&settings](const QString& key,
                                         const auto value) noexcept {
    if (!settings.contains(key)) {
      settings.setValue(key, value);
    }
  };

  set_if_absent("visuals-grid", true);
}

}  // namespace

TactileApplication::TactileApplication(int argc, char** argv)
    : QApplication{argc, argv},
      m_window{std::make_unique<TactileWindow>()},
      m_editor{std::make_unique<TactileEditor>()}
{
  init_surface_format();

  setStyle(QStyleFactory::create("Fusion"));
  setApplicationVersion("0.1.0");
  setOrganizationName("albinjohansson");
  setApplicationName("Tactile");
  //  setOrganizationDomain("https://github.com/albin-johansson/Tactile");

  QIcon icon{":/res/icons/tactile2_icon2.png"};
  setWindowIcon(icon);

  load_style_sheet(":/res/tactile_light.qss");

  init_connections();
  validate_settings();

  m_window->showMaximized();
}

TactileApplication::~TactileApplication() noexcept = default;

void TactileApplication::load_style_sheet(const char* styleSheet)
{
  if (!styleSheet) {
    return;
  }

  QFile file{styleSheet};
  file.open(QFile::ReadOnly);
  QString StyleSheet = QLatin1String(file.readAll());
  setStyleSheet(StyleSheet);
}

void TactileApplication::init_connections() noexcept
{
  using W = TactileWindow;
  using E = TactileEditor;

  auto* window = m_window.get();
  auto* editor = m_editor.get();

  connect(window, &W::req_add_row, editor, &E::add_row);
  connect(window, &W::req_add_col, editor, &E::add_col);

  connect(window, &W::req_remove_row, editor, &E::remove_row);
  connect(window, &W::req_remove_col, editor, &E::remove_col);

  connect(window, &W::req_new_map, editor, &E::new_map);
  connect(window, &W::req_close_map, editor, &E::close_map);

  connect(window, &W::req_center_camera, this, [window, editor] {
    const auto tileSize = TileSize::get().size();
    const auto width = editor->cols() * tileSize;
    const auto height = editor->rows() * tileSize;
    window->center_camera(width, height);
  });

  connect(window, &W::req_new_tile_sheet, this, [window, editor] {
    const auto result = open_tile_sheet_image(window);
    if (result) {
      editor->add_tile_sheet(result->toStdString().c_str());
      window->enable_editor_view();
    }
  });

  connect(window, &W::req_resize_map, this, [window, editor] {
    ResizeDialog dialog;
    if (dialog.exec()) {
      editor->set_rows(*dialog.chosen_height());
      editor->set_cols(*dialog.chosen_width());
      window->trigger_redraw();
    }
  });

  connect(window, &W::req_render, editor, &E::draw);

  connect(editor, &E::updated, window, &W::trigger_redraw);
}

}  // namespace tactile