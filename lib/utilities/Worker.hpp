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
    Worker(std::function<void(void)> &&work, const std::chrono::duration<Rep, Period> &startupDelay);
    explicit Worker(std::function<void(void)> &&work);
    void waitUntilFinished() const;
    bool isRunning() const;
    void cancelStartupDelay();

  private:
    bool running;
    mutable std::mutex stateMutex;
    std::condition_variable stopCondition;
    mutable std::thread thread;
};

template <class Rep, class Period>
Worker::Worker(std::function<void(void)> &&work, const std::chrono::duration<Rep, Period> &startupDelay)
    : running(true),
      thread([&, work]() {
          std::unique_lock lock(stateMutex);
          if (!stopCondition.wait_for(lock, startupDelay, [&]() { return !running; }))
          {
              work();
          }
          running = false;
      })
{
}
