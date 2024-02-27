#include <Worker.hpp>
#include <chrono>
#include <unity.h>

using namespace std::chrono_literals;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_run_worker()
{
    bool flag = false;
    const auto work = [&flag]() {
        std::this_thread::sleep_for(1s);
        flag = true;
    };
    Worker w(work);
    w.wait_until_finished();
    TEST_ASSERT_TRUE(flag);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_run_worker);
    return UNITY_END();
}
