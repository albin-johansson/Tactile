#pragma once

#include "abstract_command.hpp"

namespace tactile::cmd {

/**
 * @class resize_map
 *
 * @brief Represents the action of resizing the active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile resize_map.hpp
 */
class resize_map final : public abstract_command
{
 public:
  /**
   * @brief Creates a `resize_map` instance.
   *
   * @param map a pointer to the associated tilemap.
   * @param nRows the new number of rows for the active tilemap.
   * @param nCols the new number of columns for the active tilemap.
   *
   * @since 0.1.0
   */
  resize_map(not_null<model::tilemap*> map, int nRows, int nCols);

  void undo() override;

  void redo() override;

 private:
  int m_rows;
  int m_cols;
  int m_oldRows{1};
  int m_oldCols{1};
};

}  // namespace tactile::cmd
