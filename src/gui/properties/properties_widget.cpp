#include "properties_widget.hpp"

#include "icons.hpp"
#include "map_tree_widget_item.hpp"
#include "preferences.hpp"
#include "tactile_qstring.hpp"
#include "tree_widget_utils.hpp"
#include "ui_properties_widget.h"

namespace tactile::gui {

inline constexpr int nameColumn = 0;
inline constexpr int valueColumn = 1;

properties_widget::properties_widget(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::properties_widget{}}
{
  m_ui->setupUi(this);

  connect(m_ui->treeWidget,
          &QTreeWidget::itemCollapsed,
          [](QTreeWidgetItem* item) {
            item->setIcon(nameColumn, icons::collapsed());
          });

  connect(m_ui->treeWidget,
          &QTreeWidget::itemExpanded,
          [](QTreeWidgetItem* item) {
            item->setIcon(nameColumn, icons::expanded());
          });

  connect(m_ui->treeWidget,
          &QTreeWidget::itemChanged,
          this,
          &properties_widget::upon_item_changed);
}

void properties_widget::selected_map(const core::map_document& document)
{
  m_props.clear();
  m_ui->treeWidget->clear();

  auto* mapItem = new map_tree_widget_item{m_ui->treeWidget};
  mapItem->update(document);

  make_tree_node(TACTILE_QSTRING(u"Custom"), m_ui->treeWidget);
}

void properties_widget::select_layer(const core::layer& layer)
{
//  m_props.clear();
//  m_ui->treeWidget->clear();
}

void properties_widget::added_property(const QString& name,
                                       const core::property& property)
{}

void properties_widget::updated_property(const QString& name,
                                         const core::property& property)
{}

void properties_widget::removed_property(const QString& name)
{}

void properties_widget::upon_item_changed(QTreeWidgetItem* item,
                                          const int column)
{
//  if (column == nameColumn) {
//    //    emit ui_rename_property("N/A", item->text(nameColumn));
//
//  } else if (column == valueColumn) {
//    if (const auto it = m_props.find(item->text(nameColumn));
//        it != m_props.end()) {
//      const auto& property = it->second;
//
//      core::property newProperty{};
//    }
//
//    const auto str = item->text(valueColumn);
//
//    // emit ui_update_property(item->text(nameColumn));
//  }
}

}  // namespace tactile::gui
