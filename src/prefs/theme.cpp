#include "theme.hpp"

#include <qapplication.h>
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>

#include "parse_palette.hpp"
#include "preferences.hpp"
#include "tactile_error.hpp"

namespace tactile::theme {
namespace {

inline constexpr QStringView light{u"Light"};
inline constexpr QStringView dark{u"Dark"};
inline constexpr QStringView atomOneDark{u"Atom One Dark"};

/**
 * @brief Indicates whether or not the supplied name is one of the
 * pre-defined themes.
 *
 * @param name the name of the theme that will be checked.
 *
 * @return `true` if the supplied name is associated with a standard theme;
 * `false` otherwise.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto is_standard_theme(QStringView name) -> bool
{
  if (name.isNull() || name.isEmpty()) {
    return false;
  }

  return light == name || dark == name || atomOneDark == name;
}

}  // namespace

void reset()
{
  set_theme(get_default_name().toString());
}

void set_theme(const QString& name)
{
  if (is_standard_theme(name)) {
    const auto palette = from_name(name);
    if (!palette) {
      qDebug() << "Failed to set theme with name: " << name;
      return;
    }

    prefs::graphics::theme().set(*palette);
    prefs::graphics::theme_name().set(name);
    QApplication::setPalette(*palette);

  } else {
    throw tactile_error{"Automatic theme parsing isn't supported yet!"};
  }
}

auto from_name(QStringView name) -> std::optional<QPalette>
{
  if (dark == name) {
    return get_dark();
  } else if (atomOneDark == name) {
    return get_atom_one_dark();
  } else if (light == name) {
    return get_light();
  } else {
    return std::nullopt;
  }
}

auto get_dark() -> const QPalette&
{
  static const auto dark = json::parse_palette(QStringLiteral(":theme/dark"));
  return dark;
}

auto get_light() -> const QPalette&
{
  static const auto light = json::parse_palette(QStringLiteral(":theme/light"));
  return light;
}

auto get_atom_one_dark() -> const QPalette&
{
  static const auto oneDark =
      json::parse_palette(QStringLiteral(":theme/atomOneDark"));
  return oneDark;
}

auto get_default() -> const QPalette&
{
  return get_dark();
}

auto get_default_name() -> QStringView
{
  return dark;
}

}  // namespace tactile::theme
