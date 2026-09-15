#include "config/DotEnv.hpp"

#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

namespace
{
std::string valueOf(const char* name)
{
    const char* value = std::getenv(name);

    return value == nullptr ? "" : value;
}
} // namespace

class DotEnvTest : public testing::Test
{
  protected:
    std::filesystem::path file;

    void SetUp() override
    {
        file = std::filesystem::temp_directory_path() /
               ("calc-dotenv-" + std::to_string(::getpid()) + ".env");
    }

    void TearDown() override
    {
        std::error_code ignored;
        std::filesystem::remove(file, ignored);
    }

    void write(const std::string& content)
    {
        std::ofstream stream(file, std::ios::trunc);
        stream << content;
    }
};

TEST_F(DotEnvTest, LoadsValues)
{
    write("CALC_TEST_HOST=127.0.0.1\nCALC_TEST_PORT=9100\n");

    config::DotEnv::load(file.string());

    EXPECT_EQ(valueOf("CALC_TEST_HOST"), "127.0.0.1");
    EXPECT_EQ(valueOf("CALC_TEST_PORT"), "9100");
}

TEST_F(DotEnvTest, KeepsValueAlreadySetInEnvironment)
{
    ::setenv("CALC_TEST_KEPT", "from-environment", 1);
    write("CALC_TEST_KEPT=from-file\n");

    config::DotEnv::load(file.string());

    // Окружение важнее файла: так параметр переопределяется на один запуск.
    EXPECT_EQ(valueOf("CALC_TEST_KEPT"), "from-environment");
}

TEST_F(DotEnvTest, SkipsCommentsAndBlankLines)
{
    write("# комментарий\n\n   \nCALC_TEST_AFTER_COMMENT=ok\n");

    config::DotEnv::load(file.string());

    EXPECT_EQ(valueOf("CALC_TEST_AFTER_COMMENT"), "ok");
}

TEST_F(DotEnvTest, StripsSurroundingQuotes)
{
    write("CALC_TEST_DSN=\"host=localhost port=5432 dbname=calc\"\n");

    config::DotEnv::load(file.string());

    EXPECT_EQ(valueOf("CALC_TEST_DSN"), "host=localhost port=5432 dbname=calc");
}

TEST_F(DotEnvTest, KeepsEqualSignsInsideValue)
{
    write("CALC_TEST_PAIRS=a=1 b=2\n");

    config::DotEnv::load(file.string());

    EXPECT_EQ(valueOf("CALC_TEST_PAIRS"), "a=1 b=2");
}

TEST_F(DotEnvTest, TreatsMissingFileAsNoSettings)
{
    EXPECT_NO_THROW(config::DotEnv::load("/nonexistent/calc.env"));
    EXPECT_NO_THROW(config::DotEnv::load(""));
}
