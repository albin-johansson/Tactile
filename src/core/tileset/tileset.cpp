
#include "tileset.hpp"

#include "property_delegate.hpp"
#include "tactile_error.hpp"

namespace tactile::core {
namespace {

auto CreateSourceRectCache(const tile_id first,
                           const tile_id last,
                           const col_t tileCount,
                           const tile_width tw,
                           const tile_height th) -> Tileset::rect_map
{
  Tileset::rect_map cache;

  const auto amount = (last + 1_t) - first;
  cache.reserve(amount.get());

  const auto tileWidth = tw.get();
  const auto tileHeight = th.get();

  for (tile_id id{first}; id <= last; ++id)
  {
    const auto index = id - first;

    const auto x = (index.get() % tileCount.get()) * tileWidth;
    const auto y = (index.get() / tileCount.get()) * tileHeight;

    cache.emplace(id, QRect{x, y, tileWidth, tileHeight});
  }

  return cache;
}

}  // namespace

Tileset::Tileset(const tile_id firstId,
                 const QImage& image,
                 const tile_width tileWidth,
                 const tile_height tileHeight)
    : mImage{QPixmap::fromImage(image)}
    , mFirstId{firstId}
    , mTileWidth{tileWidth}
    , mTileHeight{tileHeight}
    , mProperties{std::make_unique<PropertyDelegate>()}
{
  if (mImage.isNull())
  {
    throw TactileError{"Cannot create tileset from null image!"};
  }

  if (mTileWidth < 1_tw || mTileHeight < 1_th)
  {
    throw TactileError{"Invalid tileset tile dimensions!"};
  }

  mRowCount = row_t{Height() / mTileHeight.get()};
  mColumnCount = col_t{Width() / mTileWidth.get()};
  mTileCount = mRowCount.get() * mColumnCount.get();
  mLastId = mFirstId + tile_id{mTileCount};
  mSourceRects = CreateSourceRectCache(mFirstId,
                                       mLastId,
                                       mColumnCount,
                                       mTileWidth,
                                       mTileHeight);
}

Tileset::Tileset(const tile_id firstID,
                 const QString& path,
                 const tile_width tileWidth,
                 const tile_height tileHeight)
    : Tileset{firstID, QImage{path}, tileWidth, tileHeight}
{
  mPath = QFileInfo{path};
}

void Tileset::SetSelection(const TilesetSelection& selection)
{
  mSelection = selection;
}

void Tileset::ClearSelection() noexcept
{
  mSelection.reset();
}

void Tileset::SetName(QString name)
{
  mName = std::move(name);
}

void Tileset::SetPath(QFileInfo path)
{
  mPath = std::move(path);
}

auto Tileset::Contains(const tile_id id) const noexcept -> bool
{
  return (id >= FirstId()) && (id <= LastId());
}

auto Tileset::IsSingleTileSelected() const noexcept -> bool
{
  return mSelection && (mSelection->topLeft == mSelection->bottomRight);
}

auto Tileset::TileAt(const MapPosition& position) const -> tile_id
{
  const auto [row, col] = position.Unpack();

  const auto endRow = row_t{RowCount()};
  const auto endCol = col_t{ColumnCount()};

  if ((row >= 0_row) && (col >= 0_col) && (row < endRow) && (col < endCol))
  {
    const auto index = row.get() * mColumnCount.get() + col.get();
    return mFirstId + tile_id{index};
  }
  else
  {
    return empty;
  }
}

auto Tileset::Width() const -> int
{
  return mImage.width();
}

auto Tileset::Height() const -> int
{
  return mImage.height();
}

auto Tileset::ImageSource(const tile_id id) const -> Maybe<QRect>
{
  if (const auto it = mSourceRects.find(id); it != mSourceRects.end())
  {
    return it->second;
  }
  else
  {
    return nothing;
  }
}

void Tileset::AddProperty(const QString& name, const PropertyType type)
{
  mProperties->AddProperty(name, type);
}

void Tileset::AddProperty(const QString& name, const Property& property)
{
  mProperties->AddProperty(name, property);
}

void Tileset::RemoveProperty(const QString& name)
{
  mProperties->RemoveProperty(name);
}

void Tileset::RenameProperty(const QString& oldName, const QString& newName)
{
  mProperties->RenameProperty(oldName, newName);
}

void Tileset::SetProperty(const QString& name, const Property& property)
{
  mProperties->SetProperty(name, property);
}

void Tileset::ChangePropertyType(const QString& name, const PropertyType type)
{
  mProperties->ChangePropertyType(name, type);
}

auto Tileset::GetProperty(const QString& name) const -> const Property&
{
  return mProperties->GetProperty(name);
}

auto Tileset::GetProperty(const QString& name) -> Property&
{
  return mProperties->GetProperty(name);
}

auto Tileset::HasProperty(const QString& name) const -> bool
{
  return mProperties->HasProperty(name);
}

auto Tileset::PropertyCount() const noexcept -> int
{
  return mProperties->PropertyCount();
}

auto Tileset::GetProperties() const -> const property_map&
{
  return mProperties->GetProperties();
}

auto Tileset::GetName() const -> QStringView
{
  return u"Tileset";
}

}  // namespace tactile::core
