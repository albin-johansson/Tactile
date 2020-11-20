#include "open_map.hpp"

#include <QFileInfo>

#include "open_tmx.hpp"
#include "tactile_error.hpp"
#include "tiled_json_parser.hpp"

namespace tactile::service {

auto open_map(const QString& path) -> core::map_document*
{
  const QFileInfo info{path};
  const auto suffix = info.suffix();
  if (suffix == QStringLiteral(u"json")) {
    tiled_json_parser parser{info};
    if (!parser) {
      // TODO return error message and trigger modal error window!
    }
    return parser.take_document();
  } else if (suffix == QStringLiteral(u"tmx")) {
    return open_tmx_map(info);
  } else {
    throw tactile_error{"Did not recognize map format to open!"};
  }
}

}  // namespace tactile::service
