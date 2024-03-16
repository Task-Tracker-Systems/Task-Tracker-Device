#pragma once
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>

class Worker
{
  public:
    template <class Function, class Rep, class Period>
    void spawnNew(Function work, const std::chrono::duration<Rep, Period> &startupDelay);
    void cancelStartup();

    template <class Function, class Rep, class Period>
    void restart(Function work, const std::chrono::duration<Rep, Period> &startupDelay)
    {
        cancelStartup();
        spawnNew(std::forward<Function>(work), startupDelay);
    }

  private:
    bool abortFlag;
    std::mutex abortMutex;
    std::condition_variable abortCondition;
};

template <class Function, class Rep, class Period>
void Worker::spawnNew(Function work, const std::chrono::duration<Rep, Period> &startupDelay)
{
    std::thread workerThread(
        [this](auto work,
               const auto &startupDelay) {
            std::unique_lock lock(abortMutex);
            if (!abortCondition.wait_for(lock, startupDelay, [&]() { return abortFlag; }))
            {
                work();
            }

            // Problem: here lock is released, this accesses abortMutex and that may be gone
        },
        work,
        startupDelay);
    workerThread.detach();
}
