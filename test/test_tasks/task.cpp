#include "Task.hpp"
#include <ArduinoFake.h>
#include <unity.h>

static const Task::String label(L"äüöß");

using namespace fakeit;

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
    TEST_ASSERT_EQUAL_UINT(0U, task.getRecordedDuration().count());
    When(Method(ArduinoFake(), millis)).Return(0);
    task.start();
    When(Method(ArduinoFake(), millis)).Return(2000);
    task.stop();
    TEST_ASSERT_EQUAL_UINT(2U, task.getRecordedDuration().count());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_label);
    RUN_TEST(test_time_elapses);

    UNITY_END();
}
