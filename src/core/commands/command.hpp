#pragma once

#include <string>  // string

namespace tactile {

class ACommand
{
 public:
  ACommand() = default;

  explicit ACommand(std::string text);

  virtual ~ACommand() = default;

  virtual void Undo() = 0;

  virtual void Redo() = 0;

  [[nodiscard]] virtual auto GetId() const -> int = 0;

  [[nodiscard]] virtual auto Merge(const ACommand& cmd) -> bool
  {
    return false;
  }

  [[nodiscard]] auto GetText() const -> const std::string&
  {
    return mText;
  }

 private:
  std::string mText;
};

}  // namespace tactile
