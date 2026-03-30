#include "frame_test.h"
#include "projectcore.h"

FRAME_TEST(normalizedProjectNamePreservesSingleWordInput)
{
    // WHAT: Verify that a single already-normalized project token is preserved.
    // HOW: Pass a lowercase no-space string through the helper and compare the
    // result against the unchanged expected token.
    // WHY: The example frame should include at least one fast-path test so the
    // sample suite demonstrates both normalization and preservation behavior.
    FRAME_EXPECT_EQ(frame::normalizedProjectName("frame_cli"), std::string("frame_cli"));
}

int main()
{
    return FRAME_RUN_TESTS();
}
