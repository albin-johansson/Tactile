#include "tileset_widget.hpp"

#include "tileset_content_page.hpp"
#include "tileset_empty_page.hpp"
#include "ui_tileset_widget.h"

namespace tactile::gui {

tileset_widget::tileset_widget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::tileset_widget{}}

{
  m_ui->setupUi(this);

  m_contentPage = new tileset_content_page{this};
  m_emptyPage = new tileset_empty_page{this};

  m_emptyIndex = m_ui->stackedWidget->addWidget(m_emptyPage);
  m_contentIndex = m_ui->stackedWidget->addWidget(m_contentPage);

  m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);

  connect(m_emptyPage,
          &tileset_empty_page::request_new_tileset,
          this,
          &tileset_widget::request_new_tileset);

  connect(m_contentPage,
          &tileset_content_page::request_add_tileset,
          this,
          &tileset_widget::request_new_tileset);

  connect(m_contentPage,
          &tileset_content_page::selected_tileset,
          this,
          &tileset_widget::selected_tileset);

  connect(m_contentPage,
          &tileset_content_page::removed_tileset,
          this,
          &tileset_widget::removed_tileset);

  connect(m_contentPage,
          &tileset_content_page::tileset_selection_changed,
          this,
          &tileset_widget::tileset_selection_changed);

  connect(m_contentPage, &tileset_content_page::switch_to_empty_page, [this]() {
    m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);
  });
}

tileset_widget::~tileset_widget() noexcept
{
  delete m_ui;
}

void tileset_widget::add_tileset(const QImage& image,
                                 tileset_id id,
                                 tile_width tileWidth,
                                 tile_height tileHeight,
                                 const QString& tabName)
{
  if (image.isNull()) {
    return;
  }

  const auto wasEmpty = m_contentPage->empty();

  m_contentPage->add_tileset(image, id, tileWidth, tileHeight, tabName);

  if (wasEmpty) {
    m_ui->stackedWidget->setCurrentIndex(m_contentIndex);
  }
}

//void tileset_widget::remove_tileset(tileset_id id)
//{
//  m_contentPage->remove_tileset(id);
//}

}  // namespace tactile::gui
