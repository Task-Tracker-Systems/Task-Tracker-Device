#include "Worker.hpp"

void Worker::cancelStartup()
{
    abortFlag = true;
    abortCondition.notify_all();
}

std::shared_ptr<Worker> Worker::spawnNew(std::function<void(void)> &&work)
{
    return spawnNew(std::move(work), std::chrono::milliseconds(0));
}
