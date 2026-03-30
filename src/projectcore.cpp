#include "projectcore.h"

#include <algorithm>
#include <cctype>
#include <ranges>

namespace frame {

namespace {

[[nodiscard]] bool isSpace(const unsigned char ch)
{
    return std::isspace(ch) != 0;
}

} // namespace

std::string trimCopy(std::string_view text)
{
    const auto first = std::ranges::find_if_not(
        text, [](const char ch) { return isSpace(static_cast<unsigned char>(ch)); });
    const auto last = std::ranges::find_if_not(std::views::reverse(text), [](const char ch) {
                          return isSpace(static_cast<unsigned char>(ch));
                      }).base();

    if (first >= last) {
        return {};
    }

    return {first, last};
}

std::string normalizedProjectName(std::string_view rawName)
{
    std::string value = trimCopy(rawName);
    std::ranges::transform(value, value.begin(), [](const unsigned char ch) {
        if (std::isspace(ch) != 0) {
            return '_';
        }
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

} // namespace frame
