#include <gtest/gtest.h>

#include "open_map_document.hpp"
#include "tactile_qstring.hpp"
#include "tile_layer.hpp"

using namespace tactile;

TEST(ImportJsonMap, Embedded)
{
  const auto path = TACTILE_QSTRING(u"json/embedded.json");

  QObject object;
  parse::parse_error error;

  auto* document = open_map_document(path, error);
  ASSERT_TRUE(document);

  document->setParent(&object);  // Avoid memory leak

  {  // Layers
    constexpr auto rows = 5_row;
    constexpr auto cols = 7_col;

    ASSERT_EQ(2, document->LayerCount());
    ASSERT_EQ(rows, document->RowCount());
    ASSERT_EQ(cols, document->ColumnCount());

    {  // First layer
      constexpr auto id = 1_layer;
      ASSERT_TRUE(document->HasLayer(id));

      const auto* layer = document->GetLayer(id);
      ASSERT_EQ(core::LayerType::tile_layer, layer->Type());
      EXPECT_TRUE(layer->IsVisible());
      EXPECT_EQ(1.0, layer->Opacity());

      const auto* tileLayer = document->GetTileLayer(id);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(rows, tileLayer->RowCount());
      EXPECT_EQ(cols, tileLayer->ColumnCount());
      EXPECT_EQ(360_t, tileLayer->TileAt({0_row, 0_col}));
      EXPECT_EQ(360_t, tileLayer->TileAt({3_row, 0_col}));
      EXPECT_EQ(354_t, tileLayer->TileAt({0_row, 6_col}));
      EXPECT_EQ(354_t, tileLayer->TileAt({3_row, 6_col}));
    }

    {  // Second layer
      constexpr auto id = 2_layer;
      ASSERT_TRUE(document->HasLayer(id));

      const auto* layer = document->GetLayer(id);
      ASSERT_EQ(core::LayerType::tile_layer, layer->Type());
      EXPECT_TRUE(layer->IsVisible());
      EXPECT_EQ(0.8, layer->Opacity());

      const auto* tileLayer = document->GetTileLayer(id);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(empty, tileLayer->TileAt({0_row, 0_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({3_row, 0_col}));
      EXPECT_EQ(297_t, tileLayer->TileAt({0_row, 6_col}));
      EXPECT_EQ(328_t, tileLayer->TileAt({3_row, 4_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({3_row, 6_col}));
    }
  }

  {  // Tilesets
    const auto* tilesets = document->GetTilesets();
    ASSERT_TRUE(tilesets);
    ASSERT_EQ(1, tilesets->count());

    constexpr auto id = 1_ts;
    ASSERT_TRUE(tilesets->contains(id));
    EXPECT_EQ(id, tilesets->current_tileset_id());

    const auto& tileset = tilesets->at(id);
    EXPECT_EQ(1_t, tileset.first_id());
    EXPECT_EQ(TACTILE_QSTRING(u"terrain"), tileset.name());
    EXPECT_EQ(32_tw, tileset.get_tile_width());
    EXPECT_EQ(32_th, tileset.get_tile_height());
    EXPECT_EQ(1'024, tileset.tile_count());
    EXPECT_EQ(1'024, tileset.width());
    EXPECT_EQ(1'024, tileset.height());
    EXPECT_EQ(32_col, tileset.col_count());

    const QFileInfo file{TACTILE_QSTRING(u"terrain.png")};
    EXPECT_EQ(file.absoluteFilePath(), tileset.file().absoluteFilePath());
  }
}

// TODO move this to map_import_test.cpp
TEST(ImportJsonMap, External)
{
  const auto path = TACTILE_QSTRING(u"json/external.json");

  QObject object;
  parse::parse_error error;

  auto* document = open_map_document(path, error);
  ASSERT_TRUE(document);

  document->setParent(&object);  // Avoid memory leak

  constexpr auto rows = 6_row;
  constexpr auto cols = 8_col;

  {  // Layers
    ASSERT_EQ(2, document->LayerCount());
    ASSERT_EQ(rows, document->RowCount());
    ASSERT_EQ(cols, document->ColumnCount());

    {  // First layer
      constexpr auto id = 1_layer;
      ASSERT_TRUE(document->HasLayer(id));

      const auto* layer = document->GetLayer(id);
      ASSERT_EQ(core::LayerType::tile_layer, layer->Type());
      EXPECT_TRUE(layer->IsVisible());
      EXPECT_EQ(0.2, layer->Opacity());

      const auto* tileLayer = document->GetTileLayer(id);
      ASSERT_TRUE(tileLayer);

      EXPECT_EQ(rows, tileLayer->RowCount());
      EXPECT_EQ(cols, tileLayer->ColumnCount());
      EXPECT_EQ(107_t, tileLayer->TileAt({0_row, 0_col}));
      EXPECT_EQ(107_t, tileLayer->TileAt({5_row, 0_col}));
      EXPECT_EQ(372_t, tileLayer->TileAt({0_row, 7_col}));
      EXPECT_EQ(372_t, tileLayer->TileAt({5_row, 7_col}));
    }

    {  // Second layer
      constexpr auto id = 2_layer;
      ASSERT_TRUE(document->HasLayer(id));

      const auto* layer = document->GetLayer(id);
      ASSERT_EQ(core::LayerType::tile_layer, layer->Type());
      EXPECT_TRUE(layer->IsVisible());
      EXPECT_EQ(1.0, layer->Opacity());

      const auto* tileLayer = document->GetTileLayer(id);
      ASSERT_TRUE(tileLayer);
      EXPECT_EQ(1'293_t, tileLayer->TileAt({0_row, 0_col}));
      EXPECT_EQ(1'359_t, tileLayer->TileAt({2_row, 2_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({0_row, 7_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({5_row, 0_col}));
      EXPECT_EQ(empty, tileLayer->TileAt({5_row, 7_col}));
    }
  }

  {  // Tilesets
    const auto* tilesets = document->GetTilesets();
    ASSERT_TRUE(tilesets);
    ASSERT_EQ(2, tilesets->count());
    ASSERT_TRUE(tilesets->has_active_tileset());
    ASSERT_EQ(2_ts, tilesets->current_tileset_id());

    {  // First tileset
      constexpr auto id = 1_ts;
      ASSERT_TRUE(tilesets->contains(id));

      const auto& tileset = tilesets->at(id);
      EXPECT_EQ(1_t, tileset.first_id());
      EXPECT_EQ(TACTILE_QSTRING(u"terrain"), tileset.name());
      EXPECT_EQ(32_tw, tileset.get_tile_width());
      EXPECT_EQ(32_th, tileset.get_tile_height());
      EXPECT_EQ(1'024, tileset.tile_count());
      EXPECT_EQ(1'024, tileset.width());
      EXPECT_EQ(1'024, tileset.height());
      EXPECT_EQ(32_col, tileset.col_count());

      const QFileInfo file{TACTILE_QSTRING(u"terrain.png")};
      EXPECT_EQ(file.absoluteFilePath(), tileset.file().absoluteFilePath());
    }

    {  // Second tileset
      constexpr auto id = 2_ts;
      ASSERT_TRUE(tilesets->contains(id));

      const auto& tileset = tilesets->at(id);
      EXPECT_EQ(1025_t, tileset.first_id());
      EXPECT_EQ(TACTILE_QSTRING(u"outside"), tileset.name());
      EXPECT_EQ(32_tw, tileset.get_tile_width());
      EXPECT_EQ(32_th, tileset.get_tile_height());
      EXPECT_EQ(1'024, tileset.tile_count());
      EXPECT_EQ(1'024, tileset.width());
      EXPECT_EQ(1'024, tileset.height());
      EXPECT_EQ(32_col, tileset.col_count());

      const QFileInfo file{TACTILE_QSTRING(u"outside.png")};
      EXPECT_EQ(file.absoluteFilePath(), tileset.file().absoluteFilePath());
    }
  }
}
