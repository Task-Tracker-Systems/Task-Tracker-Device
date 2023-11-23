#include <command_line_interpreter.hpp>
#include <cstddef>
#include <fakeit.hpp>
#include <iostream>
#include <unity.h>

using namespace fakeit;
namespace cli = command_line_interpreter;

void setUp()
{
}

void tearDown()
{
}

static struct FooData
{
    std::size_t timesCalled;
} fooData;

static void foo()
{
    fooData.timesCalled++;
}

void test_command_argVoid()
{
    const auto myCommand = cli::makeCommand("operate", std::make_tuple(), std::function(foo));
    myCommand.execute("operate");

    TEST_ASSERT_EQUAL_UINT(1, fooData.timesCalled);
}

static struct BarData
{
    std::size_t timesCalled;
    int n;
} barData;

static void bar(const int n)
{
    barData.timesCalled++;
    barData.n = n;
}

void test_command_argInt()
{
    const cli::Option<int> barArg = {.labels = {"drinks"}, .defaultValue = -42};
    const auto myCommand2 = cli::makeCommand("bar", std::make_tuple(&barArg), std::function(bar));
    {
        barData = {};
        constexpr auto command = "bar drinks 3";
        myCommand2.execute(command);
        std::string msg = "function not called for command ";
        TEST_ASSERT_EQUAL_UINT_MESSAGE(1, barData.timesCalled, (msg + command).c_str());
        TEST_ASSERT_EQUAL_INT(3, barData.n);
    }
    {
        barData = {};
        myCommand2.execute("bar");
        TEST_ASSERT_EQUAL_UINT(1, barData.timesCalled);
        TEST_ASSERT_EQUAL_INT(-42, barData.n);
    }
    { // Test non-matching command name
        barData = {};
        myCommand2.execute("foo drinks 4");
        TEST_ASSERT_EQUAL_UINT(0, barData.timesCalled);
        TEST_ASSERT_EQUAL_INT(0, barData.n);
    }
    { // test invalid option label
        barData = {};
        try
        {
            myCommand2.execute("bar drins 3");
            TEST_FAIL_MESSAGE("exception has not been thrown for invalid label");
        }
        catch (const std::runtime_error &)
        {
        }

        TEST_ASSERT_EQUAL_UINT(0, barData.timesCalled);
        TEST_ASSERT_EQUAL_INT(0, barData.n);
    }
    { // test invalid option value
        barData = {};
        try
        {
            myCommand2.execute("bar drinks tree");
            TEST_FAIL_MESSAGE("exception has not been thrown for 3.4");
        }
        catch (const std::runtime_error &)
        {
        }

        TEST_ASSERT_EQUAL_UINT(0, barData.timesCalled);
        TEST_ASSERT_EQUAL_INT(0, barData.n);
    }
    { // Test empty command line
        barData = {};
        try
        {
            myCommand2.execute("");
            TEST_FAIL_MESSAGE("exception has not been thrown");
        }
        catch (const std::runtime_error &)
        {
        }

        TEST_ASSERT_EQUAL_UINT(0, barData.timesCalled);
        TEST_ASSERT_EQUAL_INT(0, barData.n);
    }
}

static struct
{
    std::size_t timesCalled;
    std::string s;
    int n;
} twoData;

static void two(const std::string s, const int n)
{
    twoData.timesCalled++;
    twoData.s = s;
    twoData.n = n;
}

void test_command_argStringInt()
{
    const cli::Option<std::string> object = {.labels = {"thing"}, "nothing"};
    const cli::Option<int> number = {.labels = {"number"}, 0};
    const auto myCommand3 = cli::makeCommand("two", std::make_tuple(&object, &number), std::function(two));

    {
        twoData = {};
        myCommand3.execute("two");
        TEST_ASSERT_EQUAL_STRING("nothing", twoData.s.c_str());
        TEST_ASSERT_EQUAL_INT(0, twoData.n);
    }
    {
        twoData = {};
        myCommand3.execute("two thing \"tree in the woods\" number 15");
        TEST_ASSERT_EQUAL_STRING("tree in the woods", twoData.s.c_str());
        TEST_ASSERT_EQUAL_INT(15, twoData.n);
    }
    {
        twoData = {};
        myCommand3.execute("two number -1 thing car");
        TEST_ASSERT_EQUAL_STRING("car", twoData.s.c_str());
        TEST_ASSERT_EQUAL_INT(-1, twoData.n);
    }
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_command_argVoid);
    RUN_TEST(test_command_argInt);
    RUN_TEST(test_command_argStringInt);

    UNITY_END();
}
