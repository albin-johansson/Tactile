#include "layer_utils.hpp"

namespace tactile {

auto AsTileLayer(SharedLayer& layer) -> TileLayer*
{
  if (layer->GetType() != LayerType::TileLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<TileLayer*>(layer.get());
  }
}

auto AsTileLayer(const SharedLayer& layer) -> const TileLayer*
{
  if (layer->GetType() != LayerType::TileLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<const TileLayer*>(layer.get());
  }
}

auto AsObjectLayer(SharedLayer& layer) -> ObjectLayer*
{
  if (layer->GetType() != LayerType::ObjectLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<ObjectLayer*>(layer.get());
  }
}

auto AsObjectLayer(const SharedLayer& layer) -> const ObjectLayer*
{
  if (layer->GetType() != LayerType::ObjectLayer)
  {
    return nullptr;
  }
  else
  {
    return dynamic_cast<const ObjectLayer*>(layer.get());
  }
}

}  // namespace tactile