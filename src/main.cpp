#include "projectcore.h"

#include <cstddef>
#include <iostream>
#include <iterator>
#include <span>
#include <string_view>

namespace {

void writeHelp()
{
    std::cout << "Usage: frame_cli [--help] [project-name]\n";
    std::cout << "Normalizes an example project name for deterministic output.\n";
}

} // namespace

int main(int argc, char *argv[])
{
    const std::span<char *> arguments(argv, static_cast<std::size_t>(argc));
    const bool hasSecondArgument = arguments.size() > 1U;
    const std::string_view secondArgument =
        hasSecondArgument ? std::string_view(*std::next(arguments.begin())) : std::string_view{};

    if (secondArgument == "--help") {
        writeHelp();
        return 0;
    }

    const std::string_view input =
        hasSecondArgument ? secondArgument : std::string_view("Example Project");
    std::cout << frame::normalizedProjectName(input) << '\n';
    return 0;
}
