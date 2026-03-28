#include "projectcore.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

class ProjectCoreTest final
{
public:
    void trimCopyRemovesLeadingAndTrailingWhitespace();
    void normalizedProjectNameLowercasesAndReplacesSpaces();
};

void expectEqual(const std::string &actual, const std::string &expected, const char *message)
{
    if (actual != expected) {
        throw std::runtime_error(std::string(message) + "\nexpected: " + expected + "\nactual:   " + actual);
    }
}

} // namespace

void ProjectCoreTest::trimCopyRemovesLeadingAndTrailingWhitespace()
{
    // WHAT: Verify that trimming removes leading and trailing ASCII whitespace.
    // HOW: Pass a padded string through the helper and compare the returned copy
    // against the expected core text.
    // WHY: Trimming is a common utility boundary, and deterministic behavior
    // here keeps config, CLI, and logging code from reimplementing it.
    expectEqual(frame::trimCopy("  example value \t"), "example value", "trimCopy should trim outer whitespace");
}

void ProjectCoreTest::normalizedProjectNameLowercasesAndReplacesSpaces()
{
    // WHAT: Verify that project-name normalization lowercases text and replaces
    // inner spaces with underscores.
    // HOW: Normalize a mixed-case string with spaces and compare it against the
    // expected identifier-like output.
    // WHY: The example library API is intentionally small, and this function is
    // the main deterministic behavior showcased by the frame.
    expectEqual(frame::normalizedProjectName("  Example Project Name  "), "example_project_name", "normalizedProjectName should normalize whitespace and case");
}

int main()
{
    try {
        ProjectCoreTest test;
        test.trimCopyRemovesLeadingAndTrailingWhitespace();
        test.normalizedProjectNameLowercasesAndReplacesSpaces();
        return 0;
    } catch (const std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
