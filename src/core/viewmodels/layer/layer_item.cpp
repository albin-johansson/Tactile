#include "layer_item.hpp"

#include "icons.hpp"
#include "layer_item_role.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {
namespace {

[[nodiscard]] auto icon_for_layer(const core::layer_type type) -> const QIcon&
{
  switch (type)
  {
    case core::layer_type::tile_layer:
      return icons::tile_layer();

    case core::layer_type::object_layer:
      return icons::object_layer();

    default:
      throw tactile_error{"Did not recognize layer type!"};
  }
}

}  // namespace

layer_item::layer_item(const QString& name) : QStandardItem{name}
{}

auto layer_item::make(const layer_id id, const core::layer& layer)
    -> layer_item*
{
  auto* item = new layer_item{layer.name()};

  item->set_id(id);
  item->set_type(layer.type());
  item->setIcon(icon_for_layer(layer.type()));

  return item;
}

void layer_item::set_id(layer_id id)
{
  setData(id.get(), static_cast<int>(layer_item_role::id));
}

void layer_item::set_type(core::layer_type type)
{
  setData(static_cast<int>(type), static_cast<int>(layer_item_role::type));
}

auto layer_item::get_id() const -> layer_id
{
  const auto role = static_cast<int>(layer_item_role::id);
  const auto id = data(role).value<int>();
  return layer_id{id};
}

auto layer_item::get_layer_type() const -> core::layer_type
{
  const auto role = static_cast<int>(layer_item_role::type);
  const auto type = data(role).value<int>();
  return core::layer_type{type};
}

}  // namespace tactile::vm