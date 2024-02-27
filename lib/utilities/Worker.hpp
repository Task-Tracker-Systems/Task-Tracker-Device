#pragma once
#include <functional>
#include <thread>

class Worker
{
  public:
    template <class F, class... Args>
    explicit Worker(F &&f, Args &&...args);
    void wait_until_finished() const;

  private:
    std::thread thread;
};

template <class F, class... Args>
Worker::Worker(F &&f, Args &&...args)
    : thread(f, args...)
{
}
