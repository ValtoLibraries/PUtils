#include "gtest/gtest.h"
#include "Csv.hpp"
#include "to_string.hpp"

struct CsvTest : testing::Test
{
    enum Fields
    {
        Age, Description
    };

    std::stringstream s{
            "phiste;21;metadeveloper\n"
                    "naliwe;24;koatla"
    };

    //                  name            age     description
    putils::Csv<';',    std::string,    int,    std::string> csv{ s };
};

TEST_F(CsvTest, Get)
{
    const auto &t = csv.get("phiste");
    EXPECT_EQ(std::get<Age>(t), 21);
    EXPECT_EQ(std::get<Description>(t), "metadeveloper");

    const auto &t2 = csv.get("naliwe");
    EXPECT_EQ(std::get<Age>(t2), 24);
    EXPECT_EQ(std::get<Description>(t2), "koatla");
}

TEST_F(CsvTest, GetRows)
{
    const auto &rows = csv.getRows();
    EXPECT_EQ(rows[0]->first, "phiste");

    const auto &t = rows[0]->second;
    EXPECT_EQ(std::get<Age>(t), 21);
    EXPECT_EQ(std::get<Description>(t), "metadeveloper");
}

TEST_F(CsvTest, Dump)
{
    std::stringstream s;
    csv.dump(s);
    EXPECT_EQ(s.str(),
              "phiste;21;metadeveloper\n"
                      "naliwe;24;koatla\n"
    );
}

TEST_F(CsvTest, Parse)
{
    std::string output;
    csv.parse(
            // Key action
            [&output](const std::string &key) { output += key; },
            // Fields actions
            {
                    [&output](int age) { output += putils::to_string(age); },
                    [&output](const std::string &description) { output += description; }
            },
            // End of row action
            [&output] { output += "--"; }
    );

    EXPECT_EQ(output, "phiste21metadeveloper--naliwe24koatla--");
}
