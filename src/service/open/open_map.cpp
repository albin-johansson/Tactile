#include "open_map.hpp"

#include <QFileInfo>

#include "json_engine.hpp"
#include "map_parser.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "xml_engine.hpp"

namespace tactile::service {
namespace {

template <typename T>
auto open_using(const QFileInfo& file, parse::parse_error* error)
    -> core::map_document*
{
  parse::map_parser<T> parser{file};
  *error = parser.error_code();
  if (parser) {
    return parser.make_document();
  } else {
    return nullptr;
  }
}

}  // namespace

auto open_map(const QString& path, parse::parse_error* error)
    -> core::map_document*
{
  const QFileInfo info{path};
  const auto suffix = info.suffix();
  if (suffix == TACTILE_QSTRING(u"json")) {
    return open_using<parse::json_engine>(info, error);

  } else if (suffix == TACTILE_QSTRING(u"tmx")) {
    return open_using<parse::xml_engine>(info, error);

  } else {
    throw tactile_error{"Did not recognize map format to open!"};
  }
}

}  // namespace tactile::service
