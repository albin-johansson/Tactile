#pragma once

#include <exception>  // exception

#include "czstring.hpp"

namespace tactile {

class tactile_error final : public std::exception
{
 public:
  tactile_error() noexcept = default;

  /**
   * \brief Creates an exception with the supplied message.
   *
   * \param what the message of the exception. If the string is empty, "N/A"
   * is used.
   *
   * \since 0.1.0
   */
  explicit tactile_error(const czstring what) noexcept
      : m_what{what ? what : "N/A"}
  {}

  [[nodiscard]] auto what() const noexcept -> czstring override
  {
    return m_what;
  }

 private:
  czstring m_what{"N/A"};
};

}  // namespace tactile
