#include "Worker.hpp"

void Worker::cancelStartup()
{
    abortFlag = true;
    abortCondition.notify_all();
}

std::shared_ptr<Worker> Worker::spawnNew(std::function<void(void)> &&work)
{
    return spawnNew(work, std::chrono::milliseconds(0));
}
