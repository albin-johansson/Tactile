#pragma once

#include <QFileInfo>

#include "map_document.hpp"

namespace tactile::service {

[[nodiscard]] auto open_json_map(const QFileInfo& path) -> core::map_document*;

}
