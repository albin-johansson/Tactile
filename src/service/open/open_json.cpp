#include "open_json.hpp"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <memory>   // make_shared
#include <utility>  // move

#include "layer.hpp"
#include "tactile_error.hpp"
#include "tileset.hpp"

namespace tactile::service {
namespace {

void add_tilesets(core::map_document* document,
                  const QFileInfo& path,
                  const QJsonArray& tilesets)
{
  // TODO support external tilesets

  tileset_id id{1};
  for (const auto& elem : tilesets) {
    const auto object = elem.toObject();

    const tile_id gid{object.value(u"firstgid").toInt()};
    const tile_width tileWidth{object.value(u"tilewidth").toInt()};
    const tile_height tileHeight{object.value(u"tileheight").toInt()};

    const auto relativePath = object.value(u"image").toString();
    const auto absolutePath = path.dir().absoluteFilePath(relativePath);

    auto tileset = std::make_shared<core::tileset>(
        gid, absolutePath, tileWidth, tileHeight);

    tileset->set_name(object.value(u"name").toString());
    tileset->set_path(absolutePath);

    document->add_tileset(id, std::move(tileset));
    ++id;
  }
}

void add_layers(core::map_document* document, const QJsonArray& layers)
{
  document->remove_layers();

  bool first{true};
  for (const auto& elem : layers) {
    const auto object = elem.toObject();

    const layer_id id{object.value(u"id").toInt(-1)};
    const core::row_t rows{object.value(u"height").toInt(-1)};
    const core::col_t cols{object.value(u"width").toInt(-1)};

    core::layer layer{rows, cols};
    layer.set_visible(object.value(u"visible").toBool(true));
    // TODO set name (not yet incorporated in layer)

    const auto data = object.value(u"data").toArray();
    for (int index{0}; const auto value : data) {
      const core::position pos{core::row_t{index / cols.get()},
                               core::col_t{index % cols.get()}};
      const tile_id tile{value.toInt(empty.get())};

      layer.set_tile(pos, tile);
      ++index;
    }

    document->add_layer(id, std::move(layer));

    if (first) {
      first = false;
      document->select_layer(id);
    }
  }
}

}  // namespace

auto open_json_map(const QFileInfo& path) -> core::map_document*
{
  Q_ASSERT(path.exists());

  QFile file{path.absoluteFilePath()};
  file.open(QFile::ReadOnly | QFile::Text);

  auto json = QJsonDocument::fromJson(file.readAll());

  if (json.isNull()) {
    throw tactile_error{"Failed to open JSON map file!"};
  }

  file.close();

  const auto root = json.object();
  auto* document = new core::map_document{};

  document->set_next_layer_id(layer_id{root.value(u"nextlayerid").toInt(1)});

  add_tilesets(document, path, root.value(u"tilesets").toArray());
  add_layers(document, root.value(u"layers").toArray());

  return document;
}

}  // namespace tactile::service
