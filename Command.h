#ifndef Command_h
#define Command_h

#include <deque>
#include "WorkerJob.hpp"

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
    if (s.size() > 8)
        throw domain_error(string("\"") + s + "\" is too long for int");
    for (size_t i = 0; i < s.size(); ++i)
    {
        char c = s[i];
        if (c >= '0' && c <= '9')
            ret = ret * 10 + c - '0';
        else
            throw domain_error(string("\"") + s + "\" is not an integer");
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

class Command
{
    
public:
    
    virtual void operator () (const vector<string>& v) = 0;

};


class StartCommand: public Command
{
    
public:
    
    void operator () (const vector<string>& v)
    {
        WorkerJob::threadNum = toInt(v[1]);
        int heigth;
        int width;
        int i = 0;
        vector<int> sizes;
        Matrix<unsigned> *M = nullptr;
        if (v.size() > 4 || v.size() < 3)
        {
            //
        }
        else if (v.size() == 4)
        {
            width = toInt(v[2]);
            heigth = toInt(v[3]);
            M = getRandomMatrix(width, heigth);
            M->show();
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
        auto TMP = new Matrix<unsigned>(width, heigth);
        deque<WorkerJob> wjs;
        for (int k = 0; k < WorkerJob::threadNum; ++k)
        {
            wjs.push_back(WorkerJob(M, TMP, 0, i, heigth - 1, i + sizes[k] - 1));
            i += sizes[k];
            tpv.v.push_back(new Thread(wjs[k]));
        }
    }
    
};





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


class RunCommand: public Command
{
    
public:
    
    void operator () (const vector<string>& v)
    {
        WorkerJob::isStopped = false;
        int n = toInt(v[1]);
        WorkerJob::leftItera = n;
        WorkerJob::iterCount = n;
        WorkerJob::isRunning.wakeAll();
    }
    
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



class ExitCommand: public Command
{
public:
    
    ExitCommand(bool* b): _toExit(b) {}
    
    void operator() (const vector<string>& v)
    {
        *_toExit = true;
        WorkerJob::isRunning.wakeAll();
        throw new exception;
    }
    
private:
    
    bool* _toExit;
    
};


#endif
