#pragma once

#include <QUndoStack>  // QUndoStack
#include <concepts>    // derived_from
#include <utility>     // forward

namespace tactile {

/**
 * \class CommandStack
 *
 * \brief Represents a stack of commands.
 *
 * \since 0.1.0
 *
 * \headerfile command_stack.hpp
 */
class CommandStack final : public QUndoStack
{
 public:
  explicit CommandStack(QObject* parent = nullptr) : QUndoStack{parent}
  {}

  /**
   * \brief Pushes a command onto the undo stack after executing it.
   *
   * \tparam T the type of the command.
   * \tparam Args the types of the arguments that will be forwarded.
   *
   * \param args the arguments that will be forwarded to the command
   * constructor.
   *
   * \since 0.1.0
   */
  template <std::derived_from<QUndoCommand> T, typename... Args>
  void Push(Args&&... args)
  {
    QUndoStack::push(new T{std::forward<Args>(args)...});
  }
};

}  // namespace tactile
