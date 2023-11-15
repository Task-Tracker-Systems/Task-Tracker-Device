#include <fakeit.hpp>
#include <iostream>
#include <unity.h>

using namespace fakeit;

void setUp()
{
}

void tearDown()
{
}

static void foo()
{
    std::cout << "Hello World" << std::endl;
}

static void bar(const int n)
{
    std::cout << "Bar: " << n + 1 << std::endl;
}

static void two(const std::string s, const int n)
{
    std::cout << "Two: " << n << " " << s << std::endl;
}

void test_command_argVoid()
{
    const auto myCommand = makeCommand("operate", std::make_tuple(), std::function(foo));
    myCommand.parse("operate");
}

void test_command_argInt()
{
    const Argument<int> barArg = {.labels = {"drinks"}, -42};
    const auto myCommand2 = makeCommand("bar", std::make_tuple(&barArg), std::function(bar));
    myCommand2.parse("bar drins 3");
    myCommand2.parse("bar drinks 3");
    myCommand2.parse("bar drinks ");
    myCommand2.parse("bar drinks");
    myCommand2.parse("bar");
    myCommand2.parse("op drinks 3");
    myCommand2.parse(" bar foo bar -55.5 drinks 3.9");
    myCommand2.parse(" bar foo bar -55.5 drinks 4.9");
}

void test_command_argStringInt()
{
    const Argument<std::string> object = {.labels = {"thing"}, "nothing"};
    const Argument<int> number = {.labels = {"number"}, 0};
    const auto myCommand3 = makeCommand("two", std::make_tuple(&object, &number), std::function(two));
    myCommand3.parse("two");
    myCommand3.parse("two thing=tree number 15");
    myCommand3.parse("two number -1 thing car");
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_command_argVoid);
    RUN_TEST(test_command_argInt);
    RUN_TEST(test_command_argStringInt);

    UNITY_END();
}
