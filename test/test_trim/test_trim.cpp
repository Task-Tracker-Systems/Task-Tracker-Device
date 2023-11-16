#include <fakeit.hpp>
#include <string>
#include <trim.hpp>
#include <unity.h>

using namespace fakeit;

void setUp()
{
}

void tearDown()
{
}

void test_nothingToTrim()
{
    std::string stringFine = "Hello World";
    const std::string stringExpected = stringFine;

    trim(stringFine);

    TEST_ASSERT_EQUAL_STRING(stringExpected.c_str(), stringFine.c_str());
}

void test_trimPrefix()
{
    const std::string stringExpected = "Hello World";
    std::string stringWithFront = "\t " + stringExpected;

    trim(stringWithFront);

    TEST_ASSERT_EQUAL_STRING(stringExpected.c_str(), stringWithFront.c_str());
}

void test_trimEmpty()
{
    const std::string stringExpected = "";
    std::string stringEmpty = stringExpected;

    trim(stringEmpty);

    TEST_ASSERT_EQUAL_STRING(stringExpected.c_str(), stringEmpty.c_str());
}

void test_trimWhitespaceOnly()
{
    const std::string stringExpected = "";
    std::string stringWhitespaces = "\t \n \r ";

    trim(stringWhitespaces);

    TEST_ASSERT_EQUAL_STRING(stringExpected.c_str(), stringWhitespaces.c_str());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_nothingToTrim);
    RUN_TEST(test_trimPrefix);
    RUN_TEST(test_trimEmpty);
    RUN_TEST(test_trimWhitespaceOnly);

    UNITY_END();
}
