#include "tasks/Task.hpp"
#include <iostream>
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

void test_task_manager()
{
    using namespace device;
    TEST_ASSERT_EQUAL_UINT(0, tasks.size());

    TEST_ASSERT_TRUE(tasks.emplace(31, L"hello earth").second);
    TEST_ASSERT_TRUE(tasks.emplace(42, L"hello mars").second);
    TEST_ASSERT_FALSE(tasks.emplace(42, L"hello venus").second); // same id as before
    TEST_ASSERT_EQUAL_UINT(2, tasks.size());

    for (auto [id, task] : tasks)
    {
        std::wcout << "Task id(" << id << ") label("
                   << task.getLabel() << ") duration("
                   << task.getRecordedDuration().count() << "s)" << std::endl;
    }

    tasks.clear();
    TEST_ASSERT_EQUAL_UINT(0, tasks.size());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_label);
    RUN_TEST(test_time_elapses);
    RUN_TEST(test_task_manager);

    UNITY_END();
}
