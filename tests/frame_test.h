#pragma once

/// @file frame_test.h
/// Zero-dependency test micro-framework for the development frame.
///
/// Usage:
///   #include "frame_test.h"
///
///   FRAME_TEST(myTest)
///   {
///       FRAME_EXPECT_EQ(1 + 1, 2);
///       FRAME_EXPECT_TRUE(true);
///   }
///
///   int main() { return FRAME_RUN_TESTS(); }

#include <cstddef>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace frame::test {

using TestFn = void (*)();

struct TestCase
{
    std::string name;
    TestFn body;
};

inline std::vector<TestCase> &registry()
{
    static std::vector<TestCase> cases;
    return cases;
}

struct Registrar
{
    Registrar(const char *name, TestFn body)
    {
        registry().push_back({.name = name, .body = body});
    }
};

struct TestFailure : std::exception
{
    std::string message;

    explicit TestFailure(std::string msg) : message(std::move(msg)) {}

    [[nodiscard]] const char *what() const noexcept override
    {
        return message.c_str();
    }
};

inline int runAll()
{
    std::size_t passed = 0;
    std::size_t failed = 0;

    for (const auto &tc : registry()) {
        try {
            tc.body();
            std::cout << "[PASS] " << tc.name << '\n';
            ++passed;
        }
        catch (const TestFailure &e) {
            std::cerr << "[FAIL] " << tc.name << ": " << e.what() << '\n';
            ++failed;
        }
        catch (const std::exception &e) {
            std::cerr << "[FAIL] " << tc.name << ": unexpected exception: " << e.what() << '\n';
            ++failed;
        }
    }

    std::cout << "---\n"
              << passed << " passed, " << failed << " failed, " << (passed + failed) << " total\n";

    return failed == 0 ? 0 : 1;
}

} // namespace frame::test

// ---------------------------------------------------------------------------
// Public macros
// ---------------------------------------------------------------------------

/// Register a test function. Use like: FRAME_TEST(name) { ... }
// NOLINTBEGIN(misc-use-anonymous-namespace,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables,bugprone-throwing-static-initialization)
#define FRAME_TEST(name)                                                                           \
    static void frameTest_##name();                                                                \
    static ::frame::test::Registrar frameReg_##name(#name, frameTest_##name);                      \
    static void frameTest_##name()
// NOLINTEND(misc-use-anonymous-namespace,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables,bugprone-throwing-static-initialization)

/// Run all registered tests and return 0 on success, 1 on failure.
#define FRAME_RUN_TESTS() ::frame::test::runAll()

// ---------------------------------------------------------------------------
// Assertion helpers — stringify via ostringstream for broad type support
// ---------------------------------------------------------------------------

// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)

#define FRAME_EXPECT_EQ(actual, expected)                                                          \
    do {                                                                                           \
        const auto &frameActual_ = (actual);                                                       \
        const auto &frameExpected_ = (expected);                                                   \
        if (!(frameActual_ == frameExpected_)) {                                                   \
            std::ostringstream frameMsg_;                                                          \
            frameMsg_ << "expected: " << frameExpected_ << "\n  actual: " << frameActual_          \
                      << "\n  at " << __FILE__ << ":" << __LINE__;                                 \
            throw ::frame::test::TestFailure(frameMsg_.str());                                     \
        }                                                                                          \
    } while (false)

#define FRAME_EXPECT_TRUE(expr)                                                                    \
    do {                                                                                           \
        if (!(expr)) {                                                                             \
            std::ostringstream frameMsg_;                                                          \
            frameMsg_ << "expected true: " #expr "\n  at " << __FILE__ << ":" << __LINE__;         \
            throw ::frame::test::TestFailure(frameMsg_.str());                                     \
        }                                                                                          \
    } while (false)

#define FRAME_EXPECT_FALSE(expr)                                                                   \
    do {                                                                                           \
        if ((expr)) {                                                                              \
            std::ostringstream frameMsg_;                                                          \
            frameMsg_ << "expected false: " #expr "\n  at " << __FILE__ << ":" << __LINE__;        \
            throw ::frame::test::TestFailure(frameMsg_.str());                                     \
        }                                                                                          \
    } while (false)

#define FRAME_EXPECT_THROWS(expr, exception_type)                                                  \
    do {                                                                                           \
        bool frameCaught_ = false;                                                                 \
        try {                                                                                      \
            (expr);                                                                                \
        }                                                                                          \
        catch (const exception_type &) {                                                           \
            frameCaught_ = true;                                                                   \
        }                                                                                          \
        catch (...) {                                                                              \
        }                                                                                          \
        if (!frameCaught_) {                                                                       \
            std::ostringstream frameMsg_;                                                          \
            frameMsg_ << "expected " #exception_type " from: " #expr "\n  at " << __FILE__ << ":"  \
                      << __LINE__;                                                                 \
            throw ::frame::test::TestFailure(frameMsg_.str());                                     \
        }                                                                                          \
    } while (false)

// NOLINTEND(cppcoreguidelines-avoid-do-while)
