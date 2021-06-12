#pragma once

#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"

TACTILE_DECLARE_UI(TilesetWidget)

namespace tactile {

class TilesetContentPage;
class TilesetEmptyPage;

class TilesetWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit TilesetWidget(QWidget* parent = nullptr);

  ~TilesetWidget() noexcept override;

 signals:
  void S_AddTileset();
  void S_RemoveTileset(tileset_id id);
  void S_SelectTileset(tileset_id id);
  void S_RenameTileset(tileset_id id, const QString& name);
  void S_SetTilesetSelection(const core::TilesetSelection& selection);

 public slots:
  void OnAddedTileset(map_id map, tileset_id id, const core::Tileset& tileset);

  void OnRemovedTileset(tileset_id id);

  void OnRenamedTileset(tileset_id id, const QString& name);

  void OnSwitchedMap(map_id id);

 private:
  Unique<Ui::TilesetWidget> mUi;
  TilesetContentPage* mContentPage{};
  TilesetEmptyPage* mEmptyPage{};
  int mEmptyIndex{};
  int mContentIndex{};
};

}  // namespace tactile
