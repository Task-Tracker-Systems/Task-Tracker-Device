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
    const Task task(0, label);
    TEST_ASSERT_EQUAL_STRING(label.c_str(), task.getLabel().c_str());
}

void test_time_elapses()
{
    Task task(1, label);
    constexpr unsigned int durationToTest = 2;
    TEST_ASSERT_FALSE(task.isRunning());
    TEST_ASSERT_EQUAL_UINT(0U, task.getRecordedDuration().count());
    task.start();
    TEST_ASSERT_TRUE(task.isRunning());
    std::this_thread::sleep_for(Task::Duration(durationToTest / 2));
    TEST_ASSERT_EQUAL_UINT(durationToTest / 2, task.getRecordedDuration().count());
    TEST_ASSERT_TRUE(task.isRunning());
    std::this_thread::sleep_for(Task::Duration(durationToTest / 2));
    TEST_ASSERT_TRUE(task.isRunning());
    task.stop();
    TEST_ASSERT_FALSE(task.isRunning());
    TEST_ASSERT_EQUAL_UINT(durationToTest, task.getRecordedDuration().count());
}

void test_task_id()
{
    const Task::ID idToTest = 2;
    Task task(idToTest, L"test");
    TEST_ASSERT_EQUAL_UINT(idToTest, task.getId());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_label);
    RUN_TEST(test_time_elapses);
    RUN_TEST(test_task_id);

    UNITY_END();
}
