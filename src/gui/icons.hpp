#pragma once

#include <QIcon>

/**
 * \namespace tactile::gui::icons
 *
 * \brief Contains a collection of functions used to obtain icons.
 *
 * \since 0.1.0
 */
namespace tactile::gui::icons {

[[nodiscard]] auto add() -> const QIcon&;

[[nodiscard]] auto remove() -> const QIcon&;

[[nodiscard]] auto rename() -> const QIcon&;

[[nodiscard]] auto duplicate() -> const QIcon&;

[[nodiscard]] auto copy() -> const QIcon&;

[[nodiscard]] auto paste() -> const QIcon&;

[[nodiscard]] auto layer() -> const QIcon&;

[[nodiscard]] auto visible() -> const QIcon&;

[[nodiscard]] auto invisible() -> const QIcon&;

[[nodiscard]] auto move_up() -> const QIcon&;

[[nodiscard]] auto move_down() -> const QIcon&;

[[nodiscard]] auto collapsed() -> const QIcon&;

[[nodiscard]] auto expanded() -> const QIcon&;

[[nodiscard]] auto object() -> const QIcon&;

[[nodiscard]] auto question_mark() -> const QIcon&;

}  // namespace tactile::gui::icons
