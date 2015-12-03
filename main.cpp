#include <iostream>
#include "Thread.hpp"
#include "Matrix.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <deque>
#include "Command.h"
#include "WorkerJob.hpp"
#include "MasterJob.h"

using namespace std;


int WorkerJob::threadCounter = 0;

int WorkerJob::threadNum = 1;

int WorkerJob::leftItera = 0;

int WorkerJob::iterCount = 0;

bool WorkerJob::isStopped = true;

bool WorkerJob::toExit = false;

ConditionalVariable WorkerJob::iterCondVar;

ConditionalVariable WorkerJob::runCondVar;

ConditionalVariable WorkerJob::blockCondVar;

Mutex WorkerJob::mtx;

Mutex WorkerJob::blockMutex;

Matrix<unsigned>* WorkerJob::lastVersion = NULL;


int main()
{
    srand(time(NULL));
    MasterJob mj;
    mj();
    return 0;
}
