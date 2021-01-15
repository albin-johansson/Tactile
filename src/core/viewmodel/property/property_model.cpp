#include "property_model.hpp"

#include <QDebug>

#include "file_value_widget.hpp"
#include "item_model_utils.hpp"
#include "make_property_item.hpp"
#include "property_items.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {
namespace {

void add_entry(const QString& name,
               QStandardItem* valueItem,
               QStandardItem* root)
{
  const auto rows = root->rowCount();
  root->setChild(rows, 0, new QStandardItem{name});
  root->setChild(rows, 1, valueItem);
}

[[nodiscard]] auto make_root_item(const QString& name)
{
  auto* item = new QStandardItem{name};

  item->setEditable(false);
  item->setSelectable(false);
  item->setColumnCount(1);

  return item;
}

}  // namespace

property_model::property_model(core::property_manager* manager, QObject* parent)
    : QStandardItemModel{parent}
    , m_manager{manager}
{
  if (!m_manager) {
    throw tactile_error{"property_model requires non-null property manager!"};
  }

  setColumnCount(2);

  m_predefinedRoot = make_root_item(tr("Predefined"));
  setItem(0, m_predefinedRoot);

  m_customRoot = make_root_item(tr("Custom"));
  setItem(1, m_customRoot);

  // clang-format off
  connect(this, &property_model::dataChanged,
          this, &property_model::when_data_changed);
  // clang-format on

  for (const auto& [name, property] : manager->properties()) {
    const auto index = add_property_to_gui(name, property, m_customRoot);
    itemFromIndex(index.siblingAtColumn(0))->setEditable(false);
  }
}

void property_model::clear_predefined()
{
  removeRows(0, m_predefinedRoot->rowCount(), m_predefinedRoot->index());
}

auto property_model::add_predefined(const QString& name,
                                    const core::property& property,
                                    bool readOnly) -> QModelIndex
{
  const auto index = add_property_to_gui(name, property, m_predefinedRoot);

  itemFromIndex(index)->setEnabled(!readOnly);
  itemFromIndex(index.siblingAtColumn(0))->setEnabled(false);

  return index;
}

auto property_model::add(const QString& name, core::property::type type)
    -> QModelIndex
{
  m_manager->add_property(name, type);

  const auto index =
      add_property_to_gui(name, m_manager->get_property(name), m_customRoot);
  itemFromIndex(index.siblingAtColumn(0))->setEditable(false);

  return index;
}

auto property_model::add(const QString& name, const core::property& property)
    -> QModelIndex
{
  Q_ASSERT(!contains_property(name));

  m_manager->add_property(name, property);

  const auto index = add_existing_property_to_gui(name);

  return index;
}

void property_model::change_type(const QString& name,
                                 const core::property::type type)
{
  if (auto* item = find_item(this, name, 0)) {
    const auto row = item->row();

    m_manager->remove_property(name);
    m_manager->add_property(name, type);

    auto& property = m_manager->get_property(name);
    property.set_default(type);

    removeRow(row, m_customRoot->index());

    auto* valueItem = make_property_item(property);
    m_customRoot->insertRow(row, {new QStandardItem{name}, valueItem});

    emit changed_type(indexFromItem(valueItem), type);
  }
}

void property_model::rename(const QString& oldName, const QString& newName)
{
  rename_property_in_gui(oldName, newName);
  m_manager->rename_property(oldName, newName);
}

void property_model::remove(const QString& name)
{
  remove_property_from_gui(name);
  m_manager->remove_property(name);
}

void property_model::set_predefined_name(const QString& name)
{
  m_predefinedRoot->setText(name);
}

auto property_model::contains_property(const QString& name) const -> bool
{
  return find_item(this, name, 0) != nullptr;
}

auto property_model::is_custom_property(const QModelIndex& index) const -> bool
{
  return parent(index) == indexFromItem(m_customRoot);
}

auto property_model::get_property(const QString& name) const
    -> const core::property&
{
  return m_manager->get_property(name);
}

void property_model::added_property(const QString& name)
{
  m_blockDataChangedSlot = true;
  add_existing_property_to_gui(name);
  m_blockDataChangedSlot = false;
}

void property_model::about_to_remove_property(const QString& name)
{
  remove_property_from_gui(name);
}

void property_model::updated_property(const QString& name)
{
  m_blockDataChangedSlot = true;

  if (auto* item = find_item(this, name, 0)) {
    const auto& property = m_manager->get_property(name);
    const auto sibling = item->index().siblingAtColumn(1);
    Q_ASSERT(sibling.isValid());

    auto* siblingItem = itemFromIndex(sibling);
    Q_ASSERT(siblingItem);

    switch (static_cast<vm::property_item_type>(siblingItem->type())) {
      case property_item_type::string: {
        siblingItem->setData(property.as_string(), Qt::EditRole);
        break;
      }
      case property_item_type::integer: {
        siblingItem->setData(property.as_integer(), Qt::EditRole);
        break;
      }
      case property_item_type::floating: {
        siblingItem->setData(property.as_floating(), Qt::EditRole);
        break;
      }
      case property_item_type::boolean: {
        siblingItem->setData(property.as_boolean(), Qt::CheckStateRole);
        break;
      }
      case property_item_type::file: {
        siblingItem->setData(property.as_file().path(),
                             vm::property_item_role::path);
        break;
      }
      case property_item_type::color: {
        siblingItem->setData(property.as_color(),
                             vm::property_item_role::color);
        break;
      }
      case property_item_type::object:
        break;  // TODO
    }
  }

  m_blockDataChangedSlot = false;
}

void property_model::renamed_property(const QString& oldName,
                                      const QString& newName)
{
  m_blockDataChangedSlot = true;
  rename_property_in_gui(oldName, newName);
  m_blockDataChangedSlot = false;
}

auto property_model::add_existing_property_to_gui(const QString& name)
    -> QModelIndex
{
  const auto index =
      add_property_to_gui(name, m_manager->get_property(name), m_customRoot);

  itemFromIndex(index.siblingAtColumn(0))->setEditable(false);

  return index;
}

void property_model::remove_property_from_gui(const QString& name)
{
  if (auto* item = find_item(this, name, 0)) {
    removeRow(item->row(), item->parent()->index());
  }
}

auto property_model::add_property_to_gui(const QString& name,
                                         const core::property& property,
                                         QStandardItem* root) -> QModelIndex
{
  auto* valueItem = make_property_item(property);

  m_blockDataChangedSlot = true;
  add_entry(name, valueItem, root);
  m_blockDataChangedSlot = false;

  const auto index = indexFromItem(valueItem);
  const auto type = property.get_type().value();
  if (type == core::property::file) {
    emit added_file(index);

  } else if (type == core::property::color) {
    emit added_color(index);
  }

  return index;
}

void property_model::rename_property_in_gui(const QString& oldName,
                                            const QString& newName)
{
  if (auto* item = find_item(this, oldName, 0)) {
    item->setText(newName);
  }
}

void property_model::set_value(const QString& name, QStandardItem* item)
{
  switch (static_cast<vm::property_item_type>(item->type())) {
    case property_item_type::string: {
      m_manager->set_property(name, item->data(Qt::EditRole).value<QString>());
      break;
    }
    case property_item_type::integer: {
      m_manager->set_property(name, item->data(Qt::EditRole).value<int>());
      break;
    }
    case property_item_type::floating: {
      m_manager->set_property(name, item->data(Qt::EditRole).value<double>());
      break;
    }
    case property_item_type::boolean: {
      m_manager->set_property(name,
                              item->data(Qt::CheckStateRole).value<bool>());
      break;
    }
    case property_item_type::file: {
      const auto path =
          item->data(vm::property_item_role::path).value<QString>();
      m_manager->set_property(name, QFileInfo{path});
      break;
    }
    case property_item_type::color: {
      const auto color =
          item->data(vm::property_item_role::color).value<QColor>();
      m_manager->set_property(name, color);
      break;
    }
    case property_item_type::object:
      break;  // TODO
  }
}

void property_model::when_data_changed(const QModelIndex& topLeft,
                                       const QModelIndex& bottomRight,
                                       const QVector<int>& roles)
{
  if (m_blockDataChangedSlot) {
    return;
  }

  //  qDebug() << "--- DATA CHANGED ---";
  //  qDebug() << "topLeft:    " << topLeft;
  //  qDebug() << "bottomRight:" << bottomRight;

  if (topLeft.parent() == m_customRoot->index()) {
    if (topLeft.column() == 1) {
      if (auto* item = itemFromIndex(topLeft)) {
        qDebug() << "--- DATA CHANGED ---";

        const auto name = m_customRoot->child(topLeft.row(), 0)->text();
        set_value(name, item);
      }
    }
  }
}

}  // namespace tactile::vm
