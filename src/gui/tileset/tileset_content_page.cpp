#include "tileset_content_page.hpp"

#include <QDebug>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabBar>

#include "icons.hpp"
#include "signal_blocker.hpp"
#include "tab_widget.hpp"
#include "tileset_tab.hpp"
#include "tileset_tab_context_menu.hpp"
#include "ui_tileset_content_page.h"

namespace tactile::gui {

tileset_content_page::tileset_content_page(QWidget* parent)
    : QWidget{parent}
    , m_ui{new Ui::tileset_content_page{}}
{
  m_ui->setupUi(this);

  m_tabWidget = new tab_widget{this};
  m_tabWidget->setTabsClosable(false);
  m_tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  add_corner_button();

  m_contextMenu = new tileset_tab_context_menu{this};
  m_ui->gridLayout->addWidget(m_tabWidget);

  connect(m_tabWidget,
          &QTabWidget::customContextMenuRequested,
          this,
          &tileset_content_page::trigger_context_menu);

  connect(m_contextMenu, &tileset_tab_context_menu::rename, [this](int index) {
    m_tabWidget->edit_tab(index);
  });

  connect(m_contextMenu, &tileset_tab_context_menu::remove, [this](int index) {
    if (auto* tab = tab_from_index(index)) {
      emit ui_remove_tileset(tab->id());
    }
  });

  connect(m_tabWidget, &tab_widget::edited_tab, [this](int index) {
    if (auto* tab = tab_from_index(index)) {
      emit ui_rename_tileset(tab->id(), m_tabWidget->tabBar()->tabText(index));
    }
  });

  connect(m_tabWidget, &QTabWidget::tabCloseRequested, [this](int index) {
    if (auto* tab = tab_from_index(index)) {
      emit ui_remove_tileset(tab->id());
    }
  });

  connect(m_tabWidget, &QTabWidget::tabBarClicked, [this](int index) {
    if (auto* tab = tab_from_index(index)) {
      emit ui_select_tileset(tab->id());
    }
  });

  connect(m_tabWidget, &QTabWidget::currentChanged, [this](int index) {
    if (index != -1) {
      current_manager().set_cached_index(index);
      if (auto* item = tab_from_index(index)) {
        emit ui_select_tileset(item->id());
      }
    } else {
      emit switch_to_empty_page();
    }
  });
}

tileset_content_page::~tileset_content_page() noexcept
{
  delete m_ui;
}

void tileset_content_page::add_corner_button()
{
  auto* button = new QPushButton{m_tabWidget};
  button->setIcon(icons::add());

  connect(button,
          &QPushButton::pressed,
          this,
          &tileset_content_page::ui_add_tileset);

  m_tabWidget->setCornerWidget(button, Qt::Corner::TopRightCorner);
}

void tileset_content_page::trigger_context_menu(const QPoint& pos)
{
  if (const auto index = m_tabWidget->tabBar()->tabAt(pos); index != -1) {
    m_contextMenu->set_tab_index(index);
    m_contextMenu->exec(mapToGlobal(pos));
  }
}

void tileset_content_page::selected_map(const map_id map)
{
  {
    signal_blocker blocker{m_tabWidget};  // avoid `ui_select_tileset`

    if (m_currentMap) {
      m_tabWidget->clear();
    }

    switch_to(map);
  }

  if (current_manager().is_empty()) {
    emit switch_to_empty_page();
  } else {
    emit switch_to_content_page();
  }
}

void tileset_content_page::added_tileset(const map_id map,
                                         const tileset_id id,
                                         const core::tileset& tileset)
{
  Q_ASSERT(!current_manager().contains(id));

  auto* tab = new tileset_tab{id, tileset, m_tabWidget};
  connect(tab,
          &tileset_tab::set_tileset_selection,
          this,
          &tileset_content_page::ui_set_tileset_selection);
  m_tabManagers.at(map).add(id, tab);

  const auto index = m_tabWidget->addTab(tab, tileset.name());
  m_tabWidget->setCurrentIndex(index);
}

void tileset_content_page::removed_tileset(const tileset_id id)
{
  auto& manager = current_manager();
  Q_ASSERT(manager.contains(id));

  m_tabWidget->removeTab(manager.index_of(id));
  manager.remove(id);
}

auto tileset_content_page::is_empty() const -> bool
{
  return m_tabWidget->count() == 0;
}

void tileset_content_page::switch_to(const map_id map)
{
  m_currentMap = map;
  const auto& manager = m_tabManagers[*m_currentMap];

  for (const auto& [_, tab] : manager) {
    m_tabWidget->addTab(tab, tab->name());
  }

  m_tabWidget->setCurrentIndex(manager.cached_index().value_or(0));
}

auto tileset_content_page::tab_from_index(const int index) -> tileset_tab*
{
  return qobject_cast<tileset_tab*>(m_tabWidget->widget(index));
}

auto tileset_content_page::current_manager() -> tileset_tab_manager&
{
  return m_tabManagers.at(m_currentMap.value());
}

}  // namespace tactile::gui
