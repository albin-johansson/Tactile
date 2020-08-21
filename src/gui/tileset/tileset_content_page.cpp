#include "tileset_content_page.hpp"

#include <QPushButton>

#include "tileset_tab.hpp"
#include "ui_tileset_content_page.h"

using tactile::model::tileset_info;

namespace tactile::gui {

tileset_content_page::tileset_content_page(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TilesetContentPageUI{}}
{
  m_ui->setupUi(this);

  //  auto* cornerBtn = new QPushButton{}; // deleted by tab widget
  //  m_ui->tabWidget->setCornerWidget(cornerBtn);
}

tileset_content_page::~tileset_content_page() noexcept
{
  delete m_ui;
}

void tileset_content_page::add_tileset(const QImage& image,
                                       tileset_id id,
                                       int tileWidth,
                                       int tileHeight,
                                       const QString& tabName) noexcept
{
  if (m_tabs.count(id)) {
    qDebug("Tried to add tileset with taken ID: %i", id.get());
  } else {
    if (!image.isNull()) {
      auto* tab = new tileset_tab{image, tileWidth, tileHeight, this};
      m_ui->tabWidget->addTab(tab, tabName);
      m_tabs.emplace(id, tab);
    }
  }
}

void tileset_content_page::remove_tileset(tileset_id id) noexcept
{
  if (m_tabs.count(id)) {
    m_tabs.erase(id);
    m_ui->tabWidget->removeTab(m_ui->tabWidget->currentIndex());
  } else {
    qDebug("Tried to remove non-existent tileset, ID: %i", id.get());
  }
}

auto tileset_content_page::empty() const noexcept -> bool
{
  return m_ui->tabWidget->count() == 0;
}

}  // namespace tactile::gui
