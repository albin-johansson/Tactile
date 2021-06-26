#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void UpdateGui(const Model& model, entt::dispatcher& dispatcher);

}  // namespace tactile