#include "layer_widget.hpp"

#include "icons.hpp"
#include "init_ui.hpp"
#include "layer_item_context_menu.hpp"
#include "layer_list_view.hpp"
#include "layer_model.hpp"
#include "layer_widget_context_menu.hpp"
#include "map_document.hpp"
#include "ui_layer_widget.h"

namespace tactile::gui {

// FIXME status bar layer combobox is not updated directly when layer name is
// changed

layer_widget::layer_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{init_ui<Ui::layer_widget>(this)}
    , m_view{new layer_list_view{this}}
    , m_addLayerMenu{new add_layer_context_menu{this}}
    , m_widgetMenu{new layer_widget_context_menu{m_addLayerMenu, this}}
    , m_itemMenu{new layer_item_context_menu{this}}
{
  m_ui->gridLayout->addWidget(m_view, 0, 1);
  m_ui->gridLayout->removeWidget(m_ui->opacitySlider);
  m_ui->gridLayout->addWidget(m_ui->opacitySlider, 0, 2);

  m_ui->removeLayerButton->setEnabled(false);
  m_ui->duplicateButton->setEnabled(false);
  m_ui->upButton->setEnabled(false);
  m_ui->downButton->setEnabled(false);
  m_ui->visibleButton->setEnabled(false);
  m_ui->opacitySlider->setEnabled(false);

  // clang-format off
  connect(m_view, &QWidget::customContextMenuRequested,
          this, &layer_widget::spawn_context_menu);

  connect(m_view, &layer_list_view::selection_changed,
          this, &layer_widget::when_selection_changed);

  connect(m_addLayerMenu, &add_layer_context_menu::add_tile_layer,
          this, &layer_widget::new_tile_layer_requested);

  connect(m_addLayerMenu, &add_layer_context_menu::add_object_layer,
          this, &layer_widget::new_object_layer_requested);

  connect(m_itemMenu, &layer_item_context_menu::toggle_visibility,
          m_ui->visibleButton, &QPushButton::toggle);

  connect(m_itemMenu, &layer_item_context_menu::move_layer_up,
          m_ui->upButton, &QPushButton::click);

  connect(m_itemMenu, &layer_item_context_menu::move_layer_down,
          m_ui->downButton, &QPushButton::click);

  connect(m_itemMenu, &layer_item_context_menu::duplicate_layer,
          m_ui->duplicateButton, &QPushButton::click);

  connect(m_itemMenu, &layer_item_context_menu::remove_layer,
          m_ui->removeLayerButton, &QPushButton::click);

  connect(m_itemMenu, &layer_item_context_menu::show_properties,
          [this] { m_model->show_properties(m_view->currentIndex()); });
  // clang-format on
}

layer_widget::~layer_widget() noexcept = default;

void layer_widget::selected_map(not_null<core::map_document*> document)
{
  Q_ASSERT(document);

  m_model = std::make_unique<vm::layer_model>(document);
  Q_ASSERT(!m_model->parent());

  m_view->setModel(m_model.get());
  Q_ASSERT(!m_model->parent());
}

void layer_widget::update_actions(const maybe<QModelIndex>& selected)
{
  if (!m_model) {
    return;
  }

  m_ui->opacitySlider->setEnabled(selected.has_value());
  m_ui->duplicateButton->setEnabled(selected.has_value());
  m_ui->visibleButton->setEnabled(selected.has_value());

  const auto nLayers = m_model->rowCount();
  const maybe<int> index = selected ? selected->row() : maybe<int>{};
  m_ui->upButton->setEnabled(index && index != 0);
  m_ui->downButton->setEnabled(index && index != nLayers - 1);
  m_ui->removeLayerButton->setEnabled(nLayers != 1);

  if (selected) {
    QSignalBlocker opacityBlocker{m_ui->opacitySlider};
    m_ui->opacitySlider->setValue(
        static_cast<int>(m_model->opacity(*selected) * 100.0));

    m_ui->visibleButton->setIcon(
        m_model->visible(*selected) ? icons::visible() : icons::invisible());

    m_model->select(*selected);
  }

  m_itemMenu->set_visibility_enabled(m_ui->visibleButton->isEnabled());
  m_itemMenu->set_remove_enabled(m_ui->removeLayerButton->isEnabled());
  m_itemMenu->set_move_up_enabled(m_ui->upButton->isEnabled());
  m_itemMenu->set_move_down_enabled(m_ui->downButton->isEnabled());
}

void layer_widget::spawn_context_menu(const QPoint& pos)
{
  if (m_view->selectionModel()->selection().empty()) {
    m_widgetMenu->exec(mapToGlobal(pos));
  } else {
    m_itemMenu->exec(mapToGlobal(pos));
  }
}

void layer_widget::when_selection_changed(maybe<QModelIndex> selected,
                                          maybe<QModelIndex>)
{
  update_actions(selected);
}

void layer_widget::new_tile_layer_requested()
{
  Q_ASSERT(m_model);
  m_model->add_tile_layer();
}

void layer_widget::new_object_layer_requested()
{
  Q_ASSERT(m_model);
  m_model->add_object_layer();
}

[[maybe_unused]] //
void layer_widget::on_newLayerButton_pressed()
{
  auto pos = m_ui->newLayerButton->pos();

  const auto size = m_ui->newLayerButton->size() / 2.0;
  pos += QPoint{size.width(), size.height()};

  m_addLayerMenu->exec(mapToGlobal(pos));
}

[[maybe_unused]] //
void layer_widget::on_removeLayerButton_pressed()
{
  m_model->remove(m_view->currentIndex());
  update_actions(m_view->currentIndex());
}

[[maybe_unused]] //
void layer_widget::on_upButton_pressed()
{
  const auto index = m_model->move_up(m_view->currentIndex());
  m_view->setCurrentIndex(index);
}

[[maybe_unused]] //
void layer_widget::on_downButton_pressed()
{
  const auto index = m_model->move_down(m_view->currentIndex());
  m_view->setCurrentIndex(index);
}

[[maybe_unused]] //
void layer_widget::on_duplicateButton_pressed()
{
  m_model->duplicate(m_view->currentIndex());
}

[[maybe_unused]] //
void layer_widget::on_visibleButton_toggled(const bool visible)
{
  m_model->set_visible(m_view->currentIndex(), visible);
  m_ui->visibleButton->setIcon(visible ? icons::visible() : icons::invisible());
}

[[maybe_unused]] //
void layer_widget::on_opacitySlider_valueChanged(const int value)
{
  m_model->set_opacity(m_view->currentIndex(),
                       static_cast<double>(value) / 100.0);
}

}  // namespace tactile::gui
