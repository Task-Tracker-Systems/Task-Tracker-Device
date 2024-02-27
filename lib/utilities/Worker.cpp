#include "Worker.hpp"

void Worker::wait_until_finished() const
{
    if (thread.joinable())
    {
        thread.join();
    }
}

bool Worker::isRunning() const
{
    std::lock_guard lock(stateMutex);
    return running;
}

void Worker::cancel()
{
    std::lock_guard lock(stateMutex);
    running = false;
    stopCondition.notify_all();
}
