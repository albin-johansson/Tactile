#include "save_map_document.hpp"

#include <QFileInfo>

#include "save_map_document_as_json.hpp"
#include "save_map_document_as_xml.hpp"
#include "tactile_error.hpp"

namespace tactile {

void SaveMapDocument(const QString& path, const core::MapDocument& document)
{
  const QFileInfo info{path};

  const auto suffix = info.suffix();
  if (suffix == QStringLiteral(u"tmx"))
  {
    SaveMapDocumentAsXml(path, document);
  }
  else if (suffix == QStringLiteral(u"json"))
  {
    SaveMapDocumentAsJson(path, document);
  }
  else
  {
    throw TactileError{"Did not recognize save format!"};
  }
}

}  // namespace tactile
