#include "projectcore.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

class CliSmokeTest final
{
public:
    void normalizedProjectNamePreservesSingleWordInput();
};

void expectEqual(const std::string &actual, const std::string &expected, const char *message)
{
    if (actual != expected) {
        throw std::runtime_error(std::string(message) + "\nexpected: " + expected + "\nactual:   " + actual);
    }
}

} // namespace

void CliSmokeTest::normalizedProjectNamePreservesSingleWordInput()
{
    // WHAT: Verify that a single already-normalized project token is preserved.
    // HOW: Pass a lowercase no-space string through the helper and compare the
    // result against the unchanged expected token.
    // WHY: The example frame should include at least one fast-path test so the
    // sample suite demonstrates both normalization and preservation behavior.
    expectEqual(frame::normalizedProjectName("frame_cli"), "frame_cli", "normalizedProjectName should preserve normalized input");
}

int main()
{
    try {
        CliSmokeTest test;
        test.normalizedProjectNamePreservesSingleWordInput();
        return 0;
    } catch (const std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
