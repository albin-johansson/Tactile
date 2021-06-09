#pragma once

#include <QPainter>  // QPainter
#include <QRectF>    // QRectF

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "position.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile {

void RenderMap(QPainter& painter,
               const core::map_document& document,
               maybe<core::position> mousePosition,
               const QRectF& exposed);

}  // namespace tactile
