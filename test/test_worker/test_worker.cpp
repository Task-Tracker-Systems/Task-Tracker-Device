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
    Worker w;
    w.wait_until_finished();
}

int main()
{
    UNITY_BEGIN();
    return UNITY_END();
}
