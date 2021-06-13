#include "layer_list_view.hpp"

namespace tactile {

LayerListView::LayerListView(QWidget* parent) : QListView{parent}
{
  setObjectName(QStringLiteral(u"LayerListView"));

  setContextMenuPolicy(Qt::CustomContextMenu);
  setSelectionBehavior(SelectRows);
  setSelectionMode(SingleSelection);
  setAlternatingRowColors(false);
}

void LayerListView::SelectQuietly(const QModelIndex& index)
{
  mQuiet = true;

  auto* selection = selectionModel();
  selection->clearSelection();
  selection->setCurrentIndex(index, QItemSelectionModel::Select);

  mQuiet = false;
}

void LayerListView::selectionChanged(const QItemSelection& selected,
                                     const QItemSelection& deselected)
{
  QListView::selectionChanged(selected, deselected);

  Q_ASSERT(selected.empty() || selected.size() == 1);
  Q_ASSERT(deselected.empty() || deselected.size() == 1);

  Maybe<QModelIndex> selectedIndex;
  for (const auto index : selected.indexes())
  {
    selectedIndex = index;
  }

  Maybe<QModelIndex> deselectedIndex;
  for (const auto index : deselected.indexes())
  {
    deselectedIndex = index;
  }

  if (!mQuiet)
  {
    emit S_SelectionChanged(selectedIndex, deselectedIndex);
  }
}

void LayerListView::dataChanged(const QModelIndex& topLeft,
                                const QModelIndex& bottomRight,
                                const QList<int>& roles)
{
  QListView::dataChanged(topLeft, bottomRight, roles);

  Q_ASSERT(topLeft == bottomRight);
  for (const auto role : roles)
  {
    if (role == Qt::ItemDataRole::EditRole)
    {
      const auto name = topLeft.data(Qt::EditRole).value<QString>();
      emit S_ChangedName(topLeft, name);
    }
  }
}

}  // namespace tactile
