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
    const auto myCommand = cli::makeCommand("operate", std::function(foo));
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
    const auto myCommand2 = cli::makeCommand("bar", std::function(bar), std::make_tuple(&barArg));
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
    const auto myCommand3 = cli::makeCommand("two", std::function(two), std::make_tuple(&object, &number));

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

static struct
{
    std::size_t timesCalled;
    int n;
} threeData;

static int inv(const int n)
{
    threeData.timesCalled++;
    threeData.n = n;
    return -n;
}

void test_command_intRetInt()
{
    const cli::Option<int> number = {.labels = {"number"}, 0};
    const auto myCommand3 = cli::makeCommand("three", std::function(inv), std::make_tuple(&number));

    {
        threeData = {};
        int retVal;
        myCommand3.execute("three number 55", &retVal);
        TEST_ASSERT_EQUAL_INT(1, threeData.timesCalled);
        TEST_ASSERT_EQUAL_INT(55, threeData.n);
        TEST_ASSERT_EQUAL_INT(-55, retVal);
    }
    {
        threeData = {};
        myCommand3.execute("three number 55", nullptr);
        TEST_ASSERT_EQUAL_INT(1, threeData.timesCalled);
        TEST_ASSERT_EQUAL_INT(55, threeData.n);
    }
    {
        threeData = {};
        myCommand3.execute("three number 55");
        TEST_ASSERT_EQUAL_INT(1, threeData.timesCalled);
        TEST_ASSERT_EQUAL_INT(55, threeData.n);
    }
}

void test_overallInterpreter()
{
    // command for list
    const auto list = []() { std::cout << "this is a list: a, b, c, ..." << std::endl; };
    const auto listCmd = cli::makeCommand("list", std::function(list));
    // command for edit
    const auto edit = [](const int id, const std::string label, const int duration) {
        std::cout << "Edit id(" << id << ") label('" << label << "') duration(" << duration << ")" << std::endl;
    };
    const cli::Option<int> id = {.labels = {"--id"}, .defaultValue = 0};
    const cli::Option<std::string> label = {.labels = {"--name"}, .defaultValue = "foo"};
    const cli::Option<int> duration = {.labels = {"--duration"}, .defaultValue = 0};
    const auto editCmd = cli::makeCommand("edit", std::function(edit), std::make_tuple(&id, &label, &duration));
    const auto commands = std::make_tuple(listCmd, editCmd);
    constexpr const char *commandLines[] = {
        "list",
        "edit --id 42 --name \"first task\" --duration 1337"};
    for (const auto line : commandLines)
    {
        std::apply([&line](const auto &...command) {
            return (command.execute(line) || ...);
        },
                   commands);
    }
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_command_argVoid);
    RUN_TEST(test_command_argInt);
    RUN_TEST(test_command_argStringInt);
    RUN_TEST(test_command_intRetInt);
    RUN_TEST(test_overallInterpreter);

    UNITY_END();
}
