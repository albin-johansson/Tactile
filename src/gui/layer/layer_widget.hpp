#pragma once

#include <QListWidgetItem>
#include <QWidget>

#include "layer.hpp"
#include "map_document.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "vector_map.hpp"

namespace Ui {
class layer_widget;
}

namespace tactile::gui {

class layer_item;
class layer_item_context_menu;

class layer_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit layer_widget(QWidget* visible = nullptr);

  ~layer_widget() noexcept override;

 signals:
  void ui_add_layer();
  void ui_remove_layer(layer_id id);
  void ui_select_layer(layer_id id);
  void ui_set_layer_opacity(layer_id id, double opacity);
  void ui_set_layer_visibility(layer_id id, bool visible);
  void ui_set_layer_name(layer_id id, const QString& name);
  void ui_move_layer_down(layer_id id);
  void ui_move_layer_up(layer_id id);
  void ui_duplicate_layer(layer_id id);

 public slots:
  void added_layer(layer_id id, const core::layer& layer);

  void added_duplicated_layer(layer_id id, const core::layer& layer);

  void removed_layer(layer_id id);

  void selected_layer(layer_id id, const core::layer& layer);

  void selected_map(map_id mapId, const core::map_document& document);

  void moved_layer_back(layer_id id);

  void moved_layer_forward(layer_id id);

 private:
  Ui::layer_widget* m_ui{};
  layer_item_context_menu* m_contextMenu{};
  vector_map<map_id, int> m_suffixes;
  maybe<map_id> m_currentMap;
  maybe<int> m_duplicateTargetRow;

  void trigger_layer_item_context_menu(const QPoint& pos);

  void add_layer(layer_id id, const core::layer& layer);

  [[nodiscard]] auto item_for_layer_id(layer_id id) -> layer_item*;

  [[nodiscard]] auto current_item() const -> const layer_item*;

  void update_possible_actions();

 private slots:
  void current_item_changed(QListWidgetItem* current,
                            QListWidgetItem* previous);

  void item_changed(QListWidgetItem* item);
};

}  // namespace tactile::gui
