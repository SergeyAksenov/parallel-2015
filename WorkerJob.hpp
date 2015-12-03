#ifndef WorkerJob_h
#define WorkerJob_h

#include "Matrix.hpp"
#include <iostream>
#include "Thread.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <deque>
#include "Command.h"
#include <ctime>
#include <cinttypes>
#include <math.h>
#include <sys/time.h>

using namespace std;


vector<int> getSizes(int thNum, int wdth)
{
    int com = wdth / thNum;
    wdth -= thNum * com;
    vector<int> v(thNum, com);
    while (wdth > 0)
    {
        v[wdth] += 1;
        --wdth;
    }
    return v;
}

vector<string> split(const string& s)
{
    vector<string> ret;
    istringstream iss(s);
    string w;
    while (iss >> w)
    if (!w.empty())
    ret.push_back(w);
    return ret;
}

int toInt(const string &s)
{
    int ret = 0;
    for (size_t i = 0; i < s.size(); ++i)
    {
        char c = s[i];
        if (c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    }
    return ret;
}

struct ThreadPointerVector
{
    vector<Thread*> v;
    
    ~ThreadPointerVector()
    {
        for (int i = 0; i < v.size(); ++i)
        {
            delete v[i];
        }
    }
};

ThreadPointerVector tpv;






class StatusCommand: public Command
{
    
public:
    
    StatusCommand(Matrix<unsigned>** M): _M(M) {}
    
    void operator () (const vector<string>& v)
    {
        (*_M)->show();
    }
    
private:
    
    Matrix<unsigned>** _M;
    
};


class StopCommand: public Command
{
public:
    
    StopCommand(bool* b): _isStopped(b) {}
    
    void operator () (const vector<string>& v)
    {
        *_isStopped = true;
    }
    
private:
    
    bool* _isStopped;
    
};




class WorkerJob: public Callable
{
    
public:
    
    WorkerJob (Matrix<unsigned>* M, Matrix<unsigned>* TMP, int ulx, int uly, int drx, int dry):
            _nextStepGrid(TMP, ulx, uly, drx, dry),
            _previousStepGrid(M, ulx, uly, drx, dry)
    {}
    
    
    void operator () ()
    {
        lastVersion = _previousStepGrid._Matrix;
        while (true)
        {
            mtx.lock();
            if (toExit)
            {
                return;
            }
            if (leftItera == 0 || isStopped)
            {
                runCondVar.wait(mtx);
            }
            mtx.unlock();
            unsigned width = _previousStepGrid.getWidth();
            unsigned heigth = _previousStepGrid.getHeigth();
            for (int i = 0; i < heigth; ++i)
            {
                for (int j = 0; j < width; ++j)
                {
                    _nextStepGrid.set(i, j, calculateNextStep(i, j));
                }
            }
            mtx.lock();
            threadCounter++;
            if (threadCounter < threadNum)
            {
                iterCondVar.wait(mtx);
            }
            else {
                threadCounter = 0;
                leftItera--;
                if (isStopped)
                {
                    lastVersion = _nextStepGrid._Matrix;
                    cout << endl << iterCount - leftItera<< " iteration number\n";
                    fflush(stdout);
                    leftItera = 0;
                }
                if (leftItera == 0)
                {
                    isStopped = true;
                    blockCondVar.wakeOne();
                    lastVersion = _nextStepGrid._Matrix;
                }
                iterCondVar.wakeAll();
            }
            _nextStepGrid.swap(_previousStepGrid);
            mtx.unlock();
        }
    }
    
    static bool toExit;
    
    static int threadCounter;
    
    static int threadNum;
    
    static int leftItera;
    
    static int iterCount;
    
    static bool isStopped;
    
    static Mutex mtx;
    
    static Mutex blockMutex;
    
    static ConditionalVariable iterCondVar;
    
    static ConditionalVariable runCondVar;
    
    static ConditionalVariable blockCondVar;
    
    static Matrix<unsigned>* lastVersion;
    
    
private:
    
    unsigned calculateNextStep (int i, int j)
    {
        int sum =
        _previousStepGrid.at(i - 1, j - 1) + _previousStepGrid.at(i, j - 1) +
        _previousStepGrid.at(i + 1, j - 1) + _previousStepGrid.at(i + 1, j) +
        _previousStepGrid.at(i + 1, j + 1) + _previousStepGrid.at(i, j + 1) +
        _previousStepGrid.at(i - 1, j + 1) + _previousStepGrid.at(i - 1, j);
        if (sum == 3 || (sum == 2 && _previousStepGrid.at(i, j) == 1))
        {
            return 1;
        }
        return 0;
    }
    
    MatrixView<unsigned> _nextStepGrid;
    
    MatrixView<unsigned> _previousStepGrid;
    
};


void StartCommand:: operator() (const vector<string>&v)
{
    WorkerJob::threadNum = toInt(v[1]);
    int heigth;
    int width;
    int i = 0;
    vector<int> sizes;
    Matrix<unsigned> *M = NULL;
    if (v.size() > 4 || v.size() < 3)
    {
        //
    }
    else if (v.size() == 4)
    {
        width = toInt(v[2]);
        heigth = toInt(v[3]);
        M = getRandomMatrix(width, heigth);
    }
    else
    {
        cout << endl << v[2] << endl;
        ifstream csv(v[2].data());
        M = fromCsv(csv);
        width = M->_width;
        heigth = M->_height;
    }
    sizes = getSizes(WorkerJob::threadNum, width);
    Matrix<unsigned>* TMP = new Matrix<unsigned>(width, heigth);
    deque<WorkerJob> wjs;
    for (int k = 0; k < WorkerJob::threadNum; ++k)
    {
        wjs.push_back(WorkerJob(M, TMP, 0, i, heigth - 1, i + sizes[k] - 1));
        i += sizes[k];
        tpv.v.push_back(new Thread(wjs[k]));
    }
}


void ExitCommand::operator() (const vector<string>& v)
{
    *_toExit = true;
    WorkerJob::runCondVar.wakeAll();
    throw new exception;
}

double getCurTime()
{
    timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000000 + t.tv_usec;
}

void TimeCommand::operator() (const vector<string>& v)
{
    if (!WorkerJob::isStopped)
    {
        throw "\nAn error occured: previous work hasn't been finished\n";
    }
    WorkerJob::isStopped = false;
    int n = toInt(v[1]);
    WorkerJob::leftItera = n;
    WorkerJob::iterCount = n;
    clock_t startTime = getCurTime();
    WorkerJob::runCondVar.wakeAll();
    WorkerJob::blockMutex.lock();
    WorkerJob::blockCondVar.wait(WorkerJob::blockMutex);
    clock_t endTime = getCurTime();
    ofstream of;
    of.open("./time.txt", ofstream::out | ofstream::app);
    of << endl << WorkerJob::threadNum <<" "<< endTime - startTime << endl;
    of.close();
    WorkerJob::blockMutex.unlock();
}

void RunCommand::operator () (const vector<string>& v)
{
    if (!WorkerJob::isStopped)
    {
        throw "\nAn error occured: previous work hasn't been finished\n";
    }
    WorkerJob::isStopped = false;
    int n = toInt(v[1]);
    WorkerJob::leftItera = n;
    WorkerJob::iterCount = n;
    WorkerJob::runCondVar.wakeAll();

}

#endif /* WorkerJob_h */
