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
    void joinIfJoinable();
    bool isRunning() const;
    void cancelStartup();
    Worker &operator=(Worker &&);
    ~Worker();

  private:
    bool running;
    std::unique_ptr<std::mutex> stateMutex;
    std::unique_ptr<std::condition_variable> stopCondition;
    std::thread thread;
};

template <class Rep, class Period>
Worker::Worker(std::function<void(void)> &&work, const std::chrono::duration<Rep, Period> &startupDelay)
    : running(true),
      stateMutex(std::make_unique<std::mutex>()),
      stopCondition(std::make_unique<std::condition_variable>()),
      thread([&, work]() {
          std::unique_lock lock(*stateMutex);
          if (!stopCondition->wait_for(lock, startupDelay, [&]() { return !running; }))
          {
              work();
          }
          running = false;
      })
{
}
