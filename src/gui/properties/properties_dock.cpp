#include "properties_dock.hpp"

#include "properties_widget.hpp"
#include "tactile_qstring.hpp"

namespace tactile {

PropertiesDock::PropertiesDock(QWidget* parent)
    : DockWidget{parent}
    , mWidget{new PropertiesWidget{this}}
{
  setObjectName(TACTILE_QSTRING(u"properties_dock"));
  setWindowTitle(tr("Properties"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);
  setWidget(mWidget);
}

void PropertiesDock::OnSwitchedMap(not_null<core::property_manager*> manager)
{
  mWidget->ShowMap(manager);
}

void PropertiesDock::ShowMapProperties(
    not_null<core::property_manager*> manager)
{
  mWidget->ShowMap(manager);
}

void PropertiesDock::ShowLayerProperties(
    not_null<core::property_manager*> manager)
{
  mWidget->ShowLayer(manager);
}

void PropertiesDock::OnAddedProperty(const QString& name)
{
  mWidget->OnAddedProperty(name);
}

void PropertiesDock::OnAboutToRemoveProperty(const QString& name)
{
  mWidget->OnAboutToRemoveProperty(name);
}

void PropertiesDock::OnUpdatedProperty(const QString& name)
{
  mWidget->OnUpdatedProperty(name);
}

void PropertiesDock::OnChangedPropertyType(const QString& name)
{
  mWidget->OnChangedPropertyType(name);
}

void PropertiesDock::OnRenamedProperty(const QString& oldName,
                                       const QString& newName)
{
  mWidget->OnRenamedProperty(oldName, newName);
}

}  // namespace tactile
