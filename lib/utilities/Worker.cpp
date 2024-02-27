#include "Worker.hpp"

Worker::Worker(std::function<void(void)> work)
    : running(true),
      thread([&, work]() {work(); running=false; })
{
}

void Worker::wait_until_finished() const
{
    if (thread.joinable())
    {
        thread.join();
    }
}

bool Worker::isRunning() const
{
    return running;
}
