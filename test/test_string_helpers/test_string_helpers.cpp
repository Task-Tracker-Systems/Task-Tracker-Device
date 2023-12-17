#include <fakeit.hpp>
#include <string>
#include <string_helpers.hpp>
#include <unity.h>

using namespace fakeit;

void setUp()
{
}

void tearDown()
{
}
void test_tokenizeString1()
{
    std::string testInput1 = "edit --name \"hello world\" --id 5";

    try
    {
        const auto tokens = tokenizeQuoted(testInput1);
        TEST_ASSERT_EQUAL_UINT(5, tokens.size());
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        TEST_FAIL_MESSAGE("tokenization is not supposed to fail");
    }
}

void test_tokenizeStringEndingQuote()
{
    std::string testInput1 = "edit --name \"hello world\"";

    try
    {
        const auto tokens = tokenizeQuoted(testInput1);
        TEST_ASSERT_EQUAL_UINT(3, tokens.size());
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        TEST_FAIL_MESSAGE("tokenization is not supposed to fail");
    }
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_tokenizeString1);
    RUN_TEST(test_tokenizeStringEndingQuote);

    UNITY_END();
}
