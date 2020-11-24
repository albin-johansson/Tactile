#pragma once

#include <QtXml>

#include "element_id.hpp"
#include "layer.hpp"
#include "map_file_type.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "tmx_element.hpp"
#include "xml_utils.hpp"

namespace tactile::tmx {

class xml_engine final
{
 public:
  using document_type = QDomDocument;
  using object_type = tmx_element;
  inline constexpr static map_file_type fileType = map_file_type::tmx;

  [[nodiscard]] auto root(const QDomDocument& document) -> object_type;

  [[nodiscard]] static auto from_file(const QFileInfo& path)
      -> maybe<QDomDocument>;

  [[nodiscard]] static auto add_tiles(core::layer& layer,
                                      const object_type& element,
                                      parse_error& error) -> bool;

  template <std::invocable<const object_type&> T>
  void each_tileset(const object_type& root, T&& callable)
  {
    const auto elements = root->elementsByTagName(QStringLiteral(u"tileset"));
    const auto count = elements.count();
    for (auto i = 0; i < count; ++i) {
      const auto& node = elements.at(i);

      const object_type elem{xml::to_elem(node)};
      Q_ASSERT(elem->tagName() == QStringView{u"tileset"});

      if (!callable(elem)) {
        return;
      }
    }
  }

  template <std::invocable<const object_type&> T>
  void each_layer(const object_type& root, T&& callable)
  {
    const auto elements = root->elementsByTagName(QStringLiteral(u"layer"));
    const auto count = elements.count();
    for (auto i = 0; i < count; ++i) {
      const auto& node = elements.at(i);

      const object_type elem{xml::to_elem(node)};
      Q_ASSERT(elem->tagName() == QStringLiteral(u"layer"));

      callable(elem);
    }
  }
};

}  // namespace tactile::tmx
