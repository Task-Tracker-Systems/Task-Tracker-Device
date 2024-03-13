#include "Worker.hpp"

void Worker::cancelStartup()
{
    std::unique_lock lock(abortMutex);
    abortFlag = true;
    abortCondition.notify_all();
}
