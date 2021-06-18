#pragma once

#include <QApplication>  // QApplication

#include "czstring.hpp"
#include "fwd.hpp"
#include "map_position.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"

namespace tactile {

/**
 * \class TactileApp
 *
 * \brief Represents the tactile application.
 *
 * \see `QApplication`
 *
 * \since 0.1.0
 *
 * \headerfile tactile_app.hpp
 */
class TactileApp final : public QApplication
{
  Q_OBJECT

 public:
  /**
   * \param argc the amount of command-line arguments.
   * \param argv the array of command-line arguments.
   *
   * \since 0.1.0
   */
  TactileApp(int argc, char** argv);

  ~TactileApp() noexcept override;

 private:
  Unique<Window> mWindow;
  Unique<core::Model> mModel;

  void InitConnections();

 private slots:
  void OnSave();

  void OnSaveAs(const QString& path);

  void OnOpenMap(const QString& path);

  void OnResizeMap();

  void OnPanUp();

  void OnPanDown();

  void OnPanRight();

  void OnPanLeft();

  void OnAddTileset();

  void OnNewMap();
};

}  // namespace tactile
