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
    , m_view{new LayerListView{this}}
    , m_addLayerMenu{new AddLayerContextMenu{this}}
    , m_widgetMenu{new LayerWidgetContextMenu{m_addLayerMenu, this}}
    , m_itemMenu{new LayerItemContextMenu{this}}
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

  connect(m_view, &LayerListView::S_SelectionChanged,
          this, &layer_widget::when_view_changed_selection);

  connect(m_view, &LayerListView::S_ChangedName,
          this, &layer_widget::when_view_changed_name);

  connect(m_addLayerMenu, &AddLayerContextMenu::S_AddTileLayer,
          this, &layer_widget::new_tile_layer_requested);

  connect(m_addLayerMenu, &AddLayerContextMenu::S_AddObjectLayer,
          this, &layer_widget::new_object_layer_requested);

  connect(m_itemMenu, &LayerItemContextMenu::S_ToggleVisibility,
          m_ui->visibleButton, &QPushButton::toggle);

  connect(m_itemMenu, &LayerItemContextMenu::S_MoveLayerUp,
          m_ui->upButton, &QPushButton::click);

  connect(m_itemMenu, &LayerItemContextMenu::S_MoveLayerDown,
          m_ui->downButton, &QPushButton::click);

  connect(m_itemMenu, &LayerItemContextMenu::S_DuplicateLayer,
          m_ui->duplicateButton, &QPushButton::click);

  connect(m_itemMenu, &LayerItemContextMenu::S_RemoveLayer,
          m_ui->removeLayerButton, &QPushButton::click);

  connect(m_itemMenu, &LayerItemContextMenu::S_ShowProperties,
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

  // clang-format off
  connect(m_model.get(), &vm::layer_model::changed_opacity,
          this, &layer_widget::changed_layer_opacity);

  connect(m_model.get(), &vm::layer_model::changed_name,
          this, &layer_widget::changed_layer_name);

  connect(m_model.get(), &vm::layer_model::changed_visibility,
          this, &layer_widget::changed_layer_visibility);

  connect(m_model.get(), &vm::layer_model::selected_layer,
          this, &layer_widget::selected_layer);
  // clang-format on
}

void layer_widget::update_actions(const maybe<QModelIndex>& selected)
{
  if (!m_model)
  {
    return;
  }

  m_ui->opacitySlider->setEnabled(selected.has_value());
  m_ui->duplicateButton->setEnabled(selected.has_value());
  m_ui->visibleButton->setEnabled(selected.has_value());

  const auto nLayers = m_model->rowCount();

  maybe<int> index;
  if (selected)
  {
    index = selected->row();
  }

  m_ui->upButton->setEnabled(index && index != 0);
  m_ui->downButton->setEnabled(index && index != nLayers - 1);
  m_ui->removeLayerButton->setEnabled(nLayers != 1);

  if (selected)
  {
    QSignalBlocker opacityBlocker{m_ui->opacitySlider};
    m_ui->opacitySlider->setValue(
        static_cast<int>(m_model->opacity(*selected) * 100.0));

    m_ui->visibleButton->setIcon(
        m_model->visible(*selected) ? icons::visible() : icons::invisible());
  }

  m_itemMenu->set_visibility_enabled(m_ui->visibleButton->isEnabled());
  m_itemMenu->set_remove_enabled(m_ui->removeLayerButton->isEnabled());
  m_itemMenu->set_move_up_enabled(m_ui->upButton->isEnabled());
  m_itemMenu->set_move_down_enabled(m_ui->downButton->isEnabled());
}

void layer_widget::spawn_context_menu(const QPoint& pos)
{
  if (m_view->selectionModel()->selection().empty())
  {
    m_widgetMenu->exec(mapToGlobal(pos));
  }
  else
  {
    m_itemMenu->exec(mapToGlobal(pos));
  }
}

void layer_widget::when_view_changed_selection(
    const maybe<QModelIndex> selected,
    const maybe<QModelIndex>)
{
  if (selected)
  {
    m_model->select(*selected);
  }
}

void layer_widget::when_view_changed_name(const QModelIndex& index,
                                          const QString& name)
{
  m_model->set_name(index, name);
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

void layer_widget::changed_layer_opacity(const layer_id, const double opacity)
{
  QSignalBlocker blocker{m_ui->opacitySlider};
  m_ui->opacitySlider->setValue(static_cast<int>(opacity * 100.0));
}

void layer_widget::changed_layer_name(const layer_id id, const QString& name)
{
  QSignalBlocker blocker{m_view};
  m_model->itemFromIndex(m_model->index_of(id).value())->setText(name);
}

void layer_widget::changed_layer_visibility(const layer_id id,
                                            const bool visible)
{
  m_ui->visibleButton->setIcon(visible ? icons::visible() : icons::invisible());
}

void layer_widget::selected_layer(const layer_id id, const core::layer& layer)
{
  const auto index = m_model->index_of(id).value();

  m_view->select_quietly(index);
  update_actions(index);
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
  const auto current = m_view->currentIndex();
  const auto next = current.siblingAtRow(current.row() - 1);

  m_model->move_up(current);
  m_view->setCurrentIndex(next);
}

[[maybe_unused]] //
void layer_widget::on_downButton_pressed()
{
  const auto current = m_view->currentIndex();
  const auto next = current.siblingAtRow(current.row() + 1);

  m_model->move_down(current);
  m_view->setCurrentIndex(next);
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
