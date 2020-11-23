#pragma once

#include <concepts>  // convertible_to, invocable

namespace tactile {

// clang-format off

template <typename T>
concept ordered = requires (T t) {
  { t < t } -> std::convertible_to<bool>;
};

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

// clang-format on

/**
 * \brief Invokes the supplied callable for the specified amount of times.
 *
 * \tparam T the type of the callable.
 *
 * \param n the amount of times the callable should be invoked.
 * \param callable the callable that will be invoked, must overload the
 * ()-operator.
 *
 * \since 0.1.0
 */
template <std::invocable T>
constexpr void invoke_n(const int n,
                        T&& callable) noexcept(noexcept(callable()))
{
  for (auto i = 0; i < n; ++i) {
    callable();
  }
}

/**
 * \brief Clamps a value to be greater than or equal to the specified threshold.
 *
 * \tparam T the type of the values.
 *
 * \param value the value that will be clamped.
 * \param least the threshold that `value` must be greater than or equal to.
 *
 * \return `value` if `value` was greater than or equal to `least`; otherwise
 * `least` is returned.
 *
 * \since 0.1.0
 */
template <ordered T>
[[nodiscard]] constexpr auto at_least(const T value, const T least) noexcept
    -> T
{
  return (value < least) ? least : value;
}

}  // namespace tactile
