#ifndef Command_h
#define Command_h

#include <deque>
#include <vector>
//#include "WorkerJob.hpp"

using namespace std;



class Command
{
    
public:
    
    virtual void operator () (const vector<string>& v) = 0;
    
};

class RunCommand: public Command
{
 
public:
 
    void operator () (const vector<string>& v);

};

class StartCommand: public Command
{
    
public:
    
    void operator () (const vector<string>& v);

};


class TimeCommand: public Command
{
    
public:
    
    void operator () (const vector<string>& v);
    
};

class ExitCommand: public Command
{
public:
    
    ExitCommand(bool* b): _toExit(b) {}
    
    void operator() (const vector<string>& v);
    
private:
    bool* _toExit;
};


#endif
