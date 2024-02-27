#pragma once
#include <atomic>
#include <functional>
#include <thread>

class Worker
{
  public:
    explicit Worker(std::function<void(void)> work);
    void wait_until_finished() const;
    bool isRunning() const;

  private:
    std::atomic<bool> running;
    mutable std::thread thread;
};
