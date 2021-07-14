#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class NewMapShortcut final : public AShortcut
{
 public:
  NewMapShortcut();

  void Activate(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile