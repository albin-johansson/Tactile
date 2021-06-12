#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "smart_pointers.hpp"

TACTILE_DECLARE_UI(TilesetEmptyPage)

namespace tactile {

class TilesetEmptyPage final : public QWidget
{
  Q_OBJECT

 public:
  explicit TilesetEmptyPage(QWidget* parent = nullptr);

  ~TilesetEmptyPage() noexcept override;

 signals:
  void S_AddTileset();

 private:
  Unique<Ui::TilesetEmptyPage> mUi;
};

}  // namespace tactile
