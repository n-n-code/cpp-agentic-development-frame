#pragma once

#include <string>
#include <string_view>

namespace frame {

/**
 * @brief Returns a trimmed copy of the input string.
 * @param text Input text.
 * @return Copy of @p text without leading or trailing ASCII whitespace.
 */
[[nodiscard]] std::string trimCopy(std::string_view text);

/**
 * @brief Produces a normalized project identifier for display or logs.
 * @param rawName User-provided or config-provided project name.
 * @return Lowercase, trimmed, underscore-separated identifier.
 */
[[nodiscard]] std::string normalizedProjectName(std::string_view rawName);

} // namespace frame
