#pragma once

#include <QFileInfo>
#include <QObject>
#include <QString>

#include "property_manager.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

/**
 * \class document
 *
 * \brief Represents a persistent document, e.g. for tilemaps.
 *
 * \see map_document
 *
 * \since 0.2.0
 *
 * \headerfile document.hpp
 */
class document : public QObject, public property_manager
{
  Q_OBJECT

 public:
  explicit document(QObject* parent = nullptr) : QObject{parent}
  {}

  ~document() noexcept override = default;

  /**
   * \brief Reverts the effects of the most recent command.
   *
   * \note This function has no effect if there is no undoable command.
   *
   * \since 0.2.0
   */
  virtual void undo() = 0;

  /**
   * \brief Executes the most recently reverted command.
   *
   * \note This function has no effect if there is no redoable command.
   *
   * \since 0.2.0
   */
  virtual void redo() = 0;

  /**
   * \brief Marks the command stack state as "clean".
   *
   * \details Marking the state of the document as clean means that this is
   * the default state, used to prevent saving a document unnecessarily when no
   * effective change has been made to it.
   *
   * \note This function should be called every time the document is saved.
   *
   * \since 0.2.0
   */
  virtual void mark_as_clean() = 0;

  /**
   * \brief Clears the entire undo/redo history associated with the document.
   *
   * \since 0.2.0
   */
  virtual void reset_history() = 0;

  /**
   * \brief Sets the file path associated with the document.
   *
   * \param path the file path of the map document.
   *
   * \since 0.2.0
   */
  virtual void set_path(QFileInfo path) = 0;

  /**
   * \brief Indicates whether or not there is an undoable command.
   *
   * \return `true` if there is an undoable command; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto can_undo() const -> bool = 0;

  /**
   * \brief Indicates whether or not there is an redoable command.
   *
   * \return `true` if there is an redoable command; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto can_redo() const -> bool = 0;

  /**
   * \brief Indicates whether or not the command state is clean.
   *
   * \return `true` if the command state is clean; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto is_clean() const -> bool = 0;

  /**
   * \brief Returns the text associated with the currently undoable command.
   *
   * \return the text associated with the currently undoable command.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto get_undo_text() const -> QString = 0;

  /**
   * \brief Returns the text associated with the currently redoable command.
   *
   * \return the text associated with the currently redoable command.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto get_redo_text() const -> QString = 0;

  /**
   * \brief Returns the file path associated with the document.
   *
   * \return the file path associated with the  document.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto path() const -> const QFileInfo& = 0;
};

}  // namespace tactile::core
