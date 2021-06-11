#pragma once

#include <QStandardItem>  // QStandardItem
#include <concepts>       // invocable

namespace tactile::vm {

template <std::invocable<QStandardItem*> T>
void VisitItems(const QStandardItemModel* model, const int column, T&& callable)
{
  const auto topLevelRows = model->rowCount();
  const auto* root = model->invisibleRootItem();

  for (auto row = 0; row < topLevelRows; ++row)
  {
    const auto* topLevelItem = root->child(row, 0);
    const auto nRows = topLevelItem->rowCount();

    for (auto subRow = 0; subRow < nRows; ++subRow)
    {
      auto* child = topLevelItem->child(subRow, column);
      if (auto* item = dynamic_cast<QStandardItem*>(child))
      {
        callable(item);
      }
    }
  }
}

[[nodiscard]] inline auto FindItem(const QStandardItemModel* model,
                                   const QString& name,
                                   const int column = 0) -> QStandardItem*
{
  const auto topLevelRows = model->rowCount();
  const auto* root = model->invisibleRootItem();

  for (auto row = 0; row < topLevelRows; ++row)
  {
    const auto* topLevelItem = root->child(row, 0);
    const auto nRows = topLevelItem->rowCount();

    for (auto subRow = 0; subRow < nRows; ++subRow)
    {
      auto* child = topLevelItem->child(subRow, column);
      auto* item = dynamic_cast<QStandardItem*>(child);
      if (item && item->text() == name)
      {
        return item;
      }
    }
  }

  return nullptr;
}

}  // namespace tactile::vm
