#pragma once

#include <filesystem>  // path, absolute

#include "aliases/unique.hpp"
#include "io/parsing/map_file.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

class MapParser final
{
 public:
  explicit MapParser(const std::filesystem::path& path);

  [[nodiscard]] explicit operator bool() const noexcept
  {
    return mError == ParseError::None;
  }

  [[nodiscard]] auto GetError() const noexcept -> ParseError
  {
    return mError;
  }

  [[nodiscard]] auto GetData() const noexcept -> const MapData&
  {
    return mData;
  }

 private:
  ParseError mError{ParseError::None};
  MapData mData;

  [[nodiscard]] auto OpenFile(const std::filesystem::path& path) -> Unique<IMapFile>;

  [[nodiscard]] auto ParseNextLayerId(const IMapFile& file) -> bool;

  [[nodiscard]] auto ParseNextObjectId(const IMapFile& file) -> bool;
};

}  // namespace Tactile::IO