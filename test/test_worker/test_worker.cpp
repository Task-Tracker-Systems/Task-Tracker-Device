#include <Worker.hpp>
#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_run_worker()
{
    bool flag = false;
    const auto work = [&flag]() { flag = true; };
    Worker w(work);
    w.wait_until_finished();
    TEST_ASSERT_TRUE(flag);
}

int main()
{
    UNITY_BEGIN();
    return UNITY_END();
}
