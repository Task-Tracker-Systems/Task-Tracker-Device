#include "Worker.hpp"

void Worker::joinIfJoinable()
{
    if (thread.joinable())
    {
        thread.join();
    }
}

bool Worker::isRunning() const
{
    std::lock_guard lock(*stateMutex);
    return running;
}

Worker::Worker(std::function<void(void)> &&work)
    : Worker(std::move(work), std::chrono::milliseconds(0))
{
}

void Worker::cancelStartup()
{
    std::lock_guard lock(*stateMutex);
    running = false;
    stopCondition->notify_all();
}

Worker &Worker::operator=(Worker &&other)
{
    // Self-assignment check
    if (this == &other)
    {
        return *this;
    }

    std::lock(*stateMutex, *(other.stateMutex));
    std::lock_guard lock_myState(*stateMutex, std::adopt_lock);            // for RAII
    std::lock_guard lock_otherState(*(other.stateMutex), std::adopt_lock); // for RAII
    std::swap(running, other.running);
    std::swap(stateMutex, other.stateMutex);
    std::swap(stopCondition, other.stopCondition);
    std::swap(thread, other.thread);

    return *this;
}

Worker::~Worker()
{
    cancelStartup();
    joinIfJoinable();
}
