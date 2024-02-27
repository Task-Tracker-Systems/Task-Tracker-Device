#include "Worker.hpp"

Worker::Worker(std::function<void(void)> work)
    : thread(work)
{
}

void Worker::wait_until_finished() const
{
    if (thread.joinable())
    {
        thread.join();
    }
}
