#include "gtest/gtest.h"
#include "CLI.hpp"

struct CLITest : testing::Test {
    putils::CLI cli {
            // Commands
            {
                    {
                            "test",
                            [](std::string_view, const std::vector<putils::CLI::Param> & params) {
                                std::cout << "Test successful" << std::endl;
                                for (const auto & p : params)
                                    std::cout << "\t[" << p.value << "]" << std::endl;
                            }
                    }
            },
            // Default
            [](std::string_view cmd, const std::vector<putils::CLI::Param> & params) {
                std::cout << "[" << cmd << "]";
                for (const auto & p : params)
                    std::cout << " [" << p.value << "]";
                std::cout << std::endl;
            },
            // Prompt
            [] { return ""; }
    };
};

TEST_F(CLITest, RegisteredCommand) {
    testing::internal::CaptureStdout();
    cli.executeCommand("test one two three");
    const auto str = testing::internal::GetCapturedStdout();
    EXPECT_EQ(str,
              "Test successful\n"
                      "\t[one]\n"
                      "\t[two]\n"
                      "\t[three]\n"
    );
}

TEST_F(CLITest, DefaultCommand) {
    testing::internal::CaptureStdout();
    cli.executeCommand("unknown one two three");
    const auto str = testing::internal::GetCapturedStdout();
    EXPECT_EQ(str, "[unknown] [one] [two] [three]\n");
}

TEST_F(CLITest, AddCommand) {
    testing::internal::CaptureStdout();
    cli.addCommand("new",
                   [](std::string_view cmd, const std::vector<putils::CLI::Param> &) {
                       std::cout << "<" << cmd << ">" << std::endl;
                   }
    );
    cli.executeCommand("new param1");
    const auto str = testing::internal::GetCapturedStdout();
    EXPECT_EQ(str, "<new>\n");
}

TEST_F(CLITest, Delimiters) {
    testing::internal::CaptureStdout();
    cli.executeCommand("unknown \"one two three\"");
    const auto str = testing::internal::GetCapturedStdout();
    EXPECT_EQ(str, "[unknown] [one two three]\n");
}

