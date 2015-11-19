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
                isRunning.wait(mtx);
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
                condvar.wait(mtx);
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
                    lastVersion = _nextStepGrid._Matrix;
                }
                condvar.wakeAll();
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
    
    static ConditionalVariable condvar;
    
    static ConditionalVariable isRunning;
    
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

#endif /* WorkerJob_h */
