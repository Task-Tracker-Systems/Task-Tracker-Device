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

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_nothingToTrim);

    UNITY_END();
}
