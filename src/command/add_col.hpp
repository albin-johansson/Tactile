#pragma once

#include "abstract_command.hpp"

namespace tactile::cmd {

/**
 * @class add_col
 *
 * @brief Represents the action of adding a column to the active map.
 *
 * @since 0.1.0
 *
 * @headerfile add_col.hpp
 */
class add_col final : public abstract_command
{
 public:
  /**
   * @brief Creates am `add_col` instance.
   *
   * @param map the associated map.
   *
   * @since 0.1.0
   */
  explicit add_col(not_null<model::map*> map);

  void undo() override;

  void redo() override;
};

}  // namespace tactile::cmd
