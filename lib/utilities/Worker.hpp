#pragma once
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

class Worker
{
  public:
    void finish()
    {
        if (delayedWorkerThread.joinable())
        {
            delayedWorkerThread.join();
        }
    }

    void cancelStartup()
    {
        if (delayedWorkerThread.joinable())
        {
            {
                std::unique_lock lock(abortMutex);
                abortFlag = true;
            }
            abortCondition.notify_all();
            delayedWorkerThread.join();
        }
    }

    template <class Function, class Rep, class Period>
    void restart(Function &&work, const std::chrono::duration<Rep, Period> &startupDelay)
    {
        cancelStartup();

        {
            std::unique_lock lock(abortMutex);
            abortFlag = false;
        }

        delayedWorkerThread = std::thread(
            [this](auto work,
                   const auto startupDelay) {
                std::unique_lock lock(abortMutex);
                if (!abortCondition.wait_for(lock, startupDelay, [&]() { return abortFlag; }))
                {
                    work();
                }
            },
            work,
            startupDelay);
    }

    ~Worker()
    {
        cancelStartup();
    }

  private:
    bool abortFlag;
    std::mutex abortMutex;
    std::condition_variable abortCondition;
    std::thread delayedWorkerThread;
};
