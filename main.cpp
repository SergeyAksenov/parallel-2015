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

bool WorkerJob::isStopped = false;

bool WorkerJob::toExit = false;

ConditionalVariable WorkerJob::condvar;

ConditionalVariable WorkerJob::isRunning;

Mutex WorkerJob::mtx;

Matrix<unsigned>* WorkerJob::lastVersion = nullptr;

struct A
{
    void operator () ()
    {
        cout << "Hello world";
        fflush(stdout);
        throw 20;
    }
};

int main()
{
    srand(time(NULL));
    string s;
    getline(cin,s);
    auto v = split(s);
    StartCommand stcmd;
    stcmd(v);
    MasterJob mj;
    mj();    return 0;
}
