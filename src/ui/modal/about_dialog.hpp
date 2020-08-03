#pragma once

#include <QDialog>

#include "basic_widget_macro.hpp"

namespace Ui {

class AboutUI;

}

namespace tactile::ui {

/**
 * The <code>AboutDialog</code> class represents the "About Tactile" dialog
 * window that displays information about the application.
 *
 * @see QDialog
 * @since 0.1.0
 */
TACTILE_BASIC_WIDGET_DECL(AboutDialog, QDialog, Ui::AboutUI)

}  // namespace tactile::ui
