#pragma once

#include <memory>  // unique_ptr

namespace tactile {

template <typename T>
using Unique = std::unique_ptr<T>;

}  // namespace tactile