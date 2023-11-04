#include "Task.hpp"
#include <thread>
#include <unity.h>

static const Task::String label(L"äüöß");

void setUp()
{
}

void tearDown()
{
}

void test_get_label()
{
    const Task task(label);
    TEST_ASSERT_EQUAL_STRING(label.c_str(), task.getLabel().c_str());
}

void test_time_elapses()
{
    Task task(label);
    constexpr unsigned int durationToTest = 2;
    TEST_ASSERT_EQUAL_UINT(0U, task.getRecordedDuration().count());
    task.start();
    std::this_thread::sleep_for(Task::Duration(durationToTest));
    task.stop();
    TEST_ASSERT_EQUAL_UINT(durationToTest, task.getRecordedDuration().count());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_label);
    RUN_TEST(test_time_elapses);

    UNITY_END();
}
