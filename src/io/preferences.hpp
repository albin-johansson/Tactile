#pragma once

#include <string>  // string

namespace Tactile {

// Loads existing persistent preferences or loads the default preferences
void LoadPreferences();

// Saves the current preferences in the persistent file directory
void SavePreferences();

namespace Prefs {

void SetPreferredFormat(std::string format);

void SetShowGrid(bool show) noexcept;

void SetEmbedTilesets(bool embed) noexcept;

void SetHumanReadableOutput(bool readable) noexcept;

[[nodiscard]] auto GetPreferredFormat() -> const std::string&;

[[nodiscard]] auto GetShowGrid() noexcept -> bool;

[[nodiscard]] auto GetEmbedTilesets() noexcept -> bool;

[[nodiscard]] auto GetHumanReadableOutput() noexcept -> bool;

}  // namespace Prefs
}  // namespace Tactile