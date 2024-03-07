#pragma once
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

class Worker
{
  public:
    template <class Rep, class Period>
    static std::shared_ptr<Worker> spawnNew(std::function<void(void)> &&work, const std::chrono::duration<Rep, Period> &startupDelay);
    static std::shared_ptr<Worker> spawnNew(std::function<void(void)> &&work);
    void cancelStartup();

  private:
    bool abortFlag;
    std::mutex abortMutex;
    std::condition_variable abortCondition;
    Worker();
};

template <class Rep, class Period>
std::shared_ptr<Worker> Worker::spawnNew(std::function<void(void)> &&work, const std::chrono::duration<Rep, Period> &startupDelay)
{
    std::shared_ptr<Worker> worker = std::make_shared<Worker>();
    std::thread workerThread(
        [](std::shared_ptr<Worker> container /* if the container goes out of scope it may call the desctructor */,
           std::function<void(void)> work,
           const std::chrono::duration<Rep, Period> startupDelay) {
            std::unique_lock lock(container->abortMutex);
            if (container->abortCondition.wait_for(lock, startupDelay, [&]() { return !container->abortFlag; }))
            {
                work();
            }
        },
        worker,
        work,
        startupDelay);
    workerThread.detach();
    return worker;
}
