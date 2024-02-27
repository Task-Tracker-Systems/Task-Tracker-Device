#pragma once
#include <functional>
#include <thread>

class Worker
{
  public:
    explicit Worker(std::function<void(void)> work);
    void wait_until_finished() const;

  private:
    std::thread thread;
};
