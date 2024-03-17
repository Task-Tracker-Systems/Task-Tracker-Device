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
        std::unique_lock _(workerThreadControlMutex);
        if (delayedWorkerThread.joinable())
        {
            delayedWorkerThread.join();
        }
    }

    void cancelStartup()
    {
        std::unique_lock _(workerThreadControlMutex);
        if (delayedWorkerThread.joinable())
        {
            {
                std::unique_lock lock(abortMutex);
                abortFlag = true;
            }
            abortSignalling.notify_all();
            delayedWorkerThread.join();
        }
    }

    template <class Function, class Rep, class Period>
    void restart(Function &&work, const std::chrono::duration<Rep, Period> &startupDelay)
    {
        std::unique_lock _(workerThreadControlMutex);
        cancelStartup();

        {
            std::unique_lock lock(abortMutex);
            abortFlag = false;
        }

        delayedWorkerThread = std::thread(
            [this](auto work,
                   const auto startupDelay) {
                std::unique_lock lock(abortMutex);
                if (!abortSignalling.wait_for(lock, startupDelay, [&]() { return abortFlag; }))
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
    bool abortFlag;                                //!< condition for aborting the work
    std::mutex abortMutex;                         //!< used to signal the abort condition to worker thread
    std::recursive_mutex workerThreadControlMutex; //!< used to protect concurrent thread ending/starting
    std::condition_variable abortSignalling;       //!< signals that it may be time to abort
    std::thread delayedWorkerThread;               //!< performs the startup delay and work
};
