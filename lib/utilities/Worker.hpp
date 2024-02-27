#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

class Worker
{
  public:
    template <class Rep, class Period>
    explicit Worker(std::function<void(void)> work, const std::chrono::duration<Rep, Period> &startDelay = 0);
    void wait_until_finished() const;
    bool isRunning() const;
    void cancel();

  private:
    bool running;
    mutable std::mutex stateMutex;
    std::condition_variable stopCondition;
    mutable std::thread thread;
};

template <class Rep, class Period>
Worker::Worker(std::function<void(void)> work, const std::chrono::duration<Rep, Period> &startDelay)
    : running(true),
      thread([&, work]() {
          std::unique_lock lock(stateMutex);
          if (!stopCondition.wait_for(lock, startDelay, [&]() { return !running; }))
          {
              work();
          }
          lock.lock();
          running = false;
      })
{
}
