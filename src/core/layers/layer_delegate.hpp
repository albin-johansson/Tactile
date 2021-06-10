#pragma once

#include <QString>  // QString

#include "layer_type.hpp"
#include "property_delegate.hpp"

namespace tactile::core {

/**
 * \class LayerDelegate
 *
 * \brief A delegate meant to be used to implement the basic layer API.
 *
 * \note This class doesn't inherit from `ILayer` because it can't provide a
 * meaningful `ILayer::Clone()` implementation.
 *
 * \since 0.2.0
 *
 * \headerfile layer_delegate.hpp
 */
class LayerDelegate final
{
 public:
  explicit LayerDelegate(layer_type type) noexcept;

  /// \name Layer API
  /// \{

  void set_visible(bool visible) noexcept;

  void set_opacity(double opacity);

  void set_name(QString name);

  [[nodiscard]] auto type() const noexcept -> layer_type;

  [[nodiscard]] auto visible() const noexcept -> bool;

  [[nodiscard]] auto opacity() const noexcept -> double;

  [[nodiscard]] auto name() const -> const QString&;

  /// \} End of layer API

  /// \name Property API
  /// \{

  void add_property(const QString& name, property_type type);

  void add_property(const QString& name, const property& property);

  void remove_property(const QString& name);

  void rename_property(const QString& oldName, const QString& newName);

  void set_property(const QString& name, const property& property);

  void change_property_type(const QString& name, property_type type);

  [[nodiscard]] auto get_property(const QString& name) const -> const property&;

  [[nodiscard]] auto get_property(const QString& name) -> property&;

  [[nodiscard]] auto has_property(const QString& name) const -> bool;

  [[nodiscard]] auto property_count() const noexcept -> int;

  [[nodiscard]] auto properties() const
      -> const property_manager::property_map&;

  /// \} End of property API

 private:
  layer_type mType;
  QString mName;
  property_delegate mProperties;
  double mOpacity{1};
  bool mVisible{true};
};

}  // namespace tactile::core
