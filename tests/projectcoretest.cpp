#include "projectcore.h"
#include "frame_test.h"

FRAME_TEST(trimCopyRemovesLeadingAndTrailingWhitespace)
{
    // WHAT: Verify that trimming removes leading and trailing ASCII whitespace.
    // HOW: Pass a padded string through the helper and compare the returned copy
    // against the expected core text.
    // WHY: Trimming is a common utility boundary, and deterministic behavior
    // here keeps config, CLI, and logging code from reimplementing it.
    FRAME_EXPECT_EQ(frame::trimCopy("  example value \t"), std::string("example value"));
}

FRAME_TEST(normalizedProjectNameLowercasesAndReplacesSpaces)
{
    // WHAT: Verify that project-name normalization lowercases text and replaces
    // inner spaces with underscores.
    // HOW: Normalize a mixed-case string with spaces and compare it against the
    // expected identifier-like output.
    // WHY: The example library API is intentionally small, and this function is
    // the main deterministic behavior showcased by the frame.
    FRAME_EXPECT_EQ(frame::normalizedProjectName("  Example Project Name  "),
                    std::string("example_project_name"));
}

int main()
{
    return FRAME_RUN_TESTS();
}
