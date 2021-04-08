#include "safethread.h"


SafeThread::~SafeThread() {
    quit();
    wait();
}
