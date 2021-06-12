#include "property_tree_view.hpp"

#include <QHeaderView>    // QHeaderView
#include <QMouseEvent>    // QMouseEvent
#include <QStandardItem>  // QStandardItem

#include "color_preview_button.hpp"
#include "file_value_widget.hpp"
#include "icons.hpp"
#include "item_model_utils.hpp"
#include "property_item_type.hpp"
#include "property_items.hpp"
#include "property_model.hpp"
#include "select_file_dialog.hpp"
#include "tactile_qstring.hpp"

namespace tactile {

PropertyTreeView::PropertyTreeView(QWidget* parent) : QTreeView{parent}
{
  setObjectName(TACTILE_QSTRING(u"property_tree_view"));

  setSelectionBehavior(SelectRows);
  setSelectionMode(SingleSelection);
  setAlternatingRowColors(true);
  setUniformRowHeights(true);
  setRootIsDecorated(false);
  setIndentation(6);

  {
    auto* h = header();
    h->setSectionResizeMode(QHeaderView::ResizeToContents);
    h->setHidden(true);
    h->setSectionsMovable(false);
  }

  // clang-format off
  connect(this, &PropertyTreeView::expanded,
          this, &PropertyTreeView::OnItemExpanded);

  connect(this, &PropertyTreeView::collapsed,
          this, &PropertyTreeView::OnItemCollapsed);
  // clang-format on
}

void PropertyTreeView::RestoreItemWidgets()
{
  vm::VisitItems(GetModel(), 1, [this](QStandardItem* item) {
    const auto itemType = static_cast<vm::PropertyItemType>(item->type());

    if (itemType == vm::PropertyItemType::Color)
    {
      OnColorAdded(item->index());
    }
    else if (itemType == vm::PropertyItemType::File)
    {
      OnFileAdded(item->index());
    }
  });
}

void PropertyTreeView::OnColorAdded(const QModelIndex& valueIndex)
{
  const auto color =
      valueIndex.data(vm::PropertyItemRole::Color).value<QColor>();
  Q_ASSERT(color.isValid());

  auto* button = new ColorPreviewButton{color};

  const auto id = NewWidgetId();
  mWidgetItems.emplace(id, GetModel()->itemFromIndex(valueIndex));

  connect(button,
          &ColorPreviewButton::S_ColorChanged,
          [this, id](const QColor& color) {
            auto* item = mWidgetItems.at(id);
            item->setData(color, vm::PropertyItemRole::Color);
          });

  setIndexWidget(valueIndex, button);
}

void PropertyTreeView::OnFileAdded(const QModelIndex& valueIndex)
{
  auto* widget = new FileValueWidget{};
  widget->SetPath(valueIndex.data(vm::PropertyItemRole::Path).value<QString>());

  const auto id = NewWidgetId();
  mWidgetItems.emplace(id, GetModel()->itemFromIndex(valueIndex));

  connect(widget, &FileValueWidget::S_SpawnDialog, [this, id] {
    SelectFileDialog::Spawn([this, id](const QString& path) {
      auto* item = mWidgetItems.at(id);
      item->setData(path, vm::PropertyItemRole::Path);

      auto* itemWidget = indexWidget(item->index());
      if (auto* widget = qobject_cast<FileValueWidget*>(itemWidget))
      {
        widget->SetPath(path);
      }
    });
  });

  setIndexWidget(valueIndex, widget);
}

void PropertyTreeView::OnChangedType(const QModelIndex& valueIndex,
                                     const core::PropertyType type)
{
  if (type == core::PropertyType::File)
  {
    OnFileAdded(valueIndex);
  }
  else if (type == core::PropertyType::Color)
  {
    OnColorAdded(valueIndex);
  }

  setCurrentIndex(valueIndex);
}

void PropertyTreeView::OnFileUpdated(const QModelIndex& index)
{
  if (auto* widget = qobject_cast<FileValueWidget*>(indexWidget(index)))
  {
    widget->SetPath(index.data(vm::PropertyItemRole::Path).value<QString>());
  }
}

void PropertyTreeView::OnColorUpdated(const QModelIndex& index)
{
  if (auto* button = qobject_cast<ColorPreviewButton*>(indexWidget(index)))
  {
    button->SetColor(index.data(vm::PropertyItemRole::Color).value<QColor>());
  }
}

void PropertyTreeView::selectionChanged(const QItemSelection& selected,
                                        const QItemSelection& deselected)
{
  QTreeView::selectionChanged(selected, deselected);

  Q_ASSERT(selected.size() == 1 || selected.empty());

  Maybe<QModelIndex> selectedIndex;
  for (const auto index : selected.indexes())
  {
    selectedIndex = index;
    if (auto* widget = qobject_cast<FileValueWidget*>(indexWidget(index)))
    {
      widget->EnterActiveMode();
    }
  }

  for (const auto index : deselected.indexes())
  {
    if (auto* widget = qobject_cast<FileValueWidget*>(indexWidget(index)))
    {
      widget->EnterIdleMode();
    }
  }

  emit S_SelectionChanged(selectedIndex);
}

void PropertyTreeView::rowsAboutToBeRemoved(const QModelIndex& parent,
                                            const int start,
                                            const int end)
{
  QTreeView::rowsAboutToBeRemoved(parent, start, end);

  // Yes, this range is inclusive: [start, end]
  for (auto row = start; row <= end; ++row)
  {
    if (const auto* item = GetModel()->item(row, 1))
    {
      if (auto* widget = indexWidget(item->index()))
      {
        widget->hide();
      }
    }
  }
}

void PropertyTreeView::mousePressEvent(QMouseEvent* event)
{
  // This is done to be able to deselect properties by pressing outside of items
  selectionModel()->clear();
  QTreeView::mousePressEvent(event);

  if (event->button() == Qt::RightButton)
  {
    emit S_SpawnContextMenu(mapToGlobal(event->pos()));
  }
}

auto PropertyTreeView::GetModel() -> vm::PropertyModel*
{
  auto* ptr = qobject_cast<vm::PropertyModel*>(model());
  Q_ASSERT(ptr);
  return ptr;
}

auto PropertyTreeView::GetModel() const -> const vm::PropertyModel*
{
  const auto* ptr = qobject_cast<const vm::PropertyModel*>(model());
  Q_ASSERT(ptr);
  return ptr;
}

auto PropertyTreeView::NewWidgetId() noexcept -> int
{
  const auto next = mNextWidgetId;
  ++mNextWidgetId;
  return next;
}

void PropertyTreeView::OnItemExpanded(const QModelIndex& index)
{
  if (!index.parent().isValid())
  {
    GetModel()->itemFromIndex(index)->setIcon(IconExpanded());
  }
}

void PropertyTreeView::OnItemCollapsed(const QModelIndex& index)
{
  if (!index.parent().isValid())
  {
    GetModel()->itemFromIndex(index)->setIcon(IconCollapsed());
  }
}

}  // namespace tactile
