#include "Task.hpp"
#include <unity.h>

void setUp()
{
}

void tearDown()
{
}

void test_get_label()
{
    const Task::String label(L"foo");
    const Task task(label);
    TEST_ASSERT_EQUAL_STRING(label.c_str(), task.getLabel().c_str());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_label);

    UNITY_END();
}
