#include <Worker.hpp>
#include <atomic>
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
    std::atomic<bool> flag = false;
    const auto work = [&flag]() {
        flag = true;
    };
    const auto worker = Worker::spawnNew(work);
    std::this_thread::sleep_for(15ms);
    TEST_ASSERT_TRUE(flag);
}

void test_run_worker2()
{
    std::atomic<bool> flag = false;
    const auto work = [&flag]() {
        flag = true;
    };
    std::shared_ptr<Worker> worker;
    Worker::restart(worker, work, 10ms);
    std::this_thread::sleep_for(20ms);
    TEST_ASSERT_TRUE(flag);
}

void test_abort()
{
    std::atomic<bool> flag = false;
    const auto work = [&flag]() {
        flag = true;
    };
    const auto worker = Worker::spawnNew(work, 100ms);
    std::this_thread::sleep_for(20ms);
    worker->cancelStartup();
    std::this_thread::sleep_for(100ms);
    TEST_ASSERT_FALSE(flag);
}

void test_abort2()
{
    std::atomic<bool> flag = false;
    const auto work = [&flag]() {
        flag = true;
    };

    std::shared_ptr<Worker> worker;
    Worker::restart(worker, work, 100ms);
    std::this_thread::sleep_for(20ms);
    Worker::restart(
        worker, []() {}, 20ms);
    std::this_thread::sleep_for(140ms);
    TEST_ASSERT_FALSE(flag);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_run_worker);
    RUN_TEST(test_run_worker2);
    RUN_TEST(test_abort);
    RUN_TEST(test_abort2);
    return UNITY_END();
}
