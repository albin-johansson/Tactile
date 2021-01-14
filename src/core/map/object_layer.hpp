#pragma once

#include "layer.hpp"
#include "layer_delegate.hpp"
#include "property_delegate.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

/**
 * \class object_layer
 *
 * \brief Represents a layer that only contains map objects.
 *
 * \since 0.2.0
 *
 * \headerfile object_layer.hpp
 */
class object_layer final : public layer
{
 public:
  object_layer();

  ~object_layer() noexcept override = default;

  /// \name Layer API

  void set_visible(bool visible) noexcept override;

  void set_opacity(double opacity) override;

  void set_name(QString name) override;

  [[nodiscard]] auto type() const -> layer_type override;

  [[nodiscard]] auto visible() const noexcept -> bool override;

  [[nodiscard]] auto opacity() const noexcept -> double override;

  [[nodiscard]] auto name() const -> const QString& override;

  [[nodiscard]] auto clone() const -> shared<layer> override;

  /// \}

  /// \name Properties
  /// \{

  void notify_property_added(const QString& name) override;

  void notify_property_removed(const QString& name) override;

  void add_property(const QString& name, property::type type) override;

  void remove_property(const QString& name) override;

  void rename_property(const QString& oldName, const QString& newName) override;

  void set_property(const QString& name, const property& property) override;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const property& override;

  [[nodiscard]] auto get_property(const QString& name) -> property& override;

  [[nodiscard]] auto property_count() const noexcept -> int override;

  [[nodiscard]] auto properties() const -> const property_map& override;

  /// \}

 private:
  // TODO m_objects
  layer_delegate m_layerDelegate;
  property_delegate m_propertyDelegate;
};

}  // namespace tactile::core
