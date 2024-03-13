#pragma once
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

class Worker
{
  public:
    template <class Rep, class Period>
    void spawnNew(std::function<void(void)> &&work, const std::chrono::duration<Rep, Period> &startupDelay);
    void cancelStartup();

    template <class Rep, class Period>
    static void restart(Worker &worker, std::function<void(void)> &&work, const std::chrono::duration<Rep, Period> &startupDelay)
    {
        worker.cancelStartup();
        worker.spawnNew(std::move(work), startupDelay);
    }

  private:
    bool abortFlag;
    std::mutex abortMutex;
    std::condition_variable abortCondition;
};

template <class Rep, class Period>
void Worker::spawnNew(std::function<void(void)> &&work, const std::chrono::duration<Rep, Period> &startupDelay)
{
    std::thread workerThread(
        [this](std::function<void(void)> work,
               const std::chrono::duration<Rep, Period> startupDelay) {
            std::unique_lock lock(abortMutex);
            if (!abortCondition.wait_for(lock, startupDelay, [&]() { return abortFlag; }))
            {
                work();
            }
        },
        work,
        startupDelay);
    workerThread.detach();
}
