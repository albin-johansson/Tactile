#pragma once

#include <QComboBox>
#include <QLabel>
#include <QStatusBar>

#include "layer_id.hpp"
#include "map_document.hpp"

namespace tactile::gui {

class status_bar final : public QStatusBar
{
  Q_OBJECT

 public:
  explicit status_bar(QWidget* parent = nullptr);

  void set_layer_combo_box_visible(bool visible);

  void set_mouse_info_visible(bool visible);

  void set_current_layer(layer_id id);

 signals:
  void select_layer_request(layer_id id);

 public slots:
  void switched_map(const core::map_document& document);

  void added_layer(layer_id id, const QString& name);

  void removed_layer(layer_id id);

  void mouse_moved(const QPointF& pos);

 private:
  QComboBox* m_layerBox{};
  QLabel* m_mxLabel{};
  QLabel* m_myLabel{};
  QLabel* m_mxValueLabel{};
  QLabel* m_myValueLabel{};
};

}  // namespace tactile::gui
