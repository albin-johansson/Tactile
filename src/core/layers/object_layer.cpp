#include "object_layer.hpp"

#include <utility>  // move

#include "tactile_qstring.hpp"

namespace tactile::core {

ObjectLayer::ObjectLayer() : mDelegate{LayerType::object_layer}
{
  mDelegate.SetName(TACTILE_QSTRING(u"Object layer"));
}

void ObjectLayer::AddProperty(const QString& name, property_type type)
{
  mDelegate.AddProperty(name, type);
}

void ObjectLayer::AddProperty(const QString& name, const property& property)
{
  mDelegate.AddProperty(name, property);
}

void ObjectLayer::RemoveProperty(const QString& name)
{
  mDelegate.RemoveProperty(name);
}

void ObjectLayer::RenameProperty(const QString& oldName,
                                  const QString& newName)
{
  mDelegate.RenameProperty(oldName, newName);
}

void ObjectLayer::SetProperty(const QString& name, const property& property)
{
  mDelegate.SetProperty(name, property);
}

void ObjectLayer::ChangePropertyType(const QString& name, property_type type)
{
  mDelegate.ChangePropertyType(name, type);
}

auto ObjectLayer::GetProperty(const QString& name) const -> const property&
{
  return mDelegate.GetProperty(name);
}

auto ObjectLayer::GetProperty(const QString& name) -> property&
{
  return mDelegate.GetProperty(name);
}

auto ObjectLayer::HasProperty(const QString& name) const -> bool
{
  return mDelegate.HasProperty(name);
}

auto ObjectLayer::PropertyCount() const noexcept -> int
{
  return mDelegate.PropertyCount();
}

auto ObjectLayer::GetProperties() const -> const property_map&
{
  return mDelegate.Properties();
}

void ObjectLayer::SetVisible(const bool visible) noexcept
{
  mDelegate.SetVisible(visible);
}

void ObjectLayer::SetOpacity(const double opacity)
{
  mDelegate.SetOpacity(opacity);
}

void ObjectLayer::SetName(QString name)
{
  mDelegate.SetName(std::move(name));
}

auto ObjectLayer::Type() const -> LayerType
{
  return mDelegate.Type();
}

auto ObjectLayer::IsVisible() const noexcept -> bool
{
  return mDelegate.Visible();
}

auto ObjectLayer::Opacity() const noexcept -> double
{
  return mDelegate.Opacity();
}

auto ObjectLayer::Name() const -> const QString&
{
  return mDelegate.Name();
}

auto ObjectLayer::Clone() const -> shared<ILayer>
{
  return std::make_shared<ObjectLayer>(*this);
}

void ObjectLayer::AddObject(const object_id id, Object obj)
{
  Q_ASSERT(!HasObject(id));
  mObjects.emplace(id, std::move(obj));
}

void ObjectLayer::AddPoint(const object_id id, const double x, const double y)
{
  Q_ASSERT(!HasObject(id));
  auto& [key, point] = mObjects.emplace(id, object_type::point);
  point.SetX(x);
  point.SetY(y);
}

void ObjectLayer::AddRectangle(const object_id id,
                               const double x,
                               const double y,
                               const double width,
                               const double height)
{
  Q_ASSERT(!HasObject(id));
  auto& [key, rect] = mObjects.emplace(id, object_type::rectangle);
  rect.SetX(x);
  rect.SetY(y);
  rect.SetWidth(width);
  rect.SetHeight(height);
}

void ObjectLayer::RemoveObject(const object_id id)
{
  Q_ASSERT(HasObject(id));
  mObjects.erase(id);
}

auto ObjectLayer::HasObject(const object_id id) const -> bool
{
  return mObjects.contains(id);
}

auto ObjectLayer::GetObject(const object_id id) -> Object&
{
  return mObjects.at(id);
}

auto ObjectLayer::GetObject(const object_id id) const -> const Object&
{
  return mObjects.at(id);
}

auto ObjectLayer::ObjectCount() const noexcept -> int
{
  return static_cast<int>(mObjects.size());
}

}  // namespace tactile::core
