#pragma once

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QWidget>

namespace Ui {

class MainWindow;

}

namespace tactile {

class EditorPane;

/**
 * The <code>TactileWindow</code> class is a subclass of
 * <code>QOpenGLWindow</code> that represents the window used in the Tactile
 * application.
 *
 * @see QOpenGLWindow
 * @since 0.1.0
 */
class TactileWindow final : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   * @since 0.1.0
   */
  explicit TactileWindow(QWidget* parent = nullptr);

  ~TactileWindow() noexcept override;

 public slots:
  /**
   * Displays the about dialog.
   * 
   * @since 0.1.0 
   */
  void display_about_dialog() noexcept;

  /**
   * Displays the settings dialog.
   * 
   * @since 0.1.0 
   */
  void display_settings_dialog() noexcept;

 private slots:
  /**
   * Exits the Tactile application.
   * 
   * @since 0.1.0 
   */
  void exit() noexcept;

 private:
  Ui::MainWindow* m_ui;
  EditorPane* m_editorPane;

  void init_connections() noexcept;
};

}  // namespace tactile
