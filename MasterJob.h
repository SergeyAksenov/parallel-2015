
#ifndef MasterJob_h
#define MasterJob_h

#include "Matrix.hpp"
#include <iostream>
#include "Thread.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <deque>
#include "Command.h"
#include <map>
#include "WorkerJob.hpp"

using namespace std;

class MasterJob: public Callable
{
    
public:
    
    MasterJob()
    {
        commandMap.insert(make_pair("TIME", new TimeCommand()));
        commandMap.insert(make_pair("STATUS", new StatusCommand(&WorkerJob::lastVersion)));
        commandMap.insert(make_pair("EXIT", new ExitCommand(&WorkerJob::toExit)));
        commandMap.insert(make_pair("STOP", new StopCommand(&WorkerJob::isStopped)));
        commandMap.insert(make_pair("START", new StartCommand()));
        commandMap.insert(make_pair("RUN", new RunCommand()));
    }
    
    void operator () ()
    {
        vector<string> vec;
        while (true)
        {
            string str;
            getline(cin, str);
            auto v = split(str);
            auto it = commandMap.find(v[0]);// check if found
            try
            {
                (*(it->second))(v);
            }
            catch (exception* e)
            {
                return;
            }
            //auto v = split(str);
            
            /*if (v[0] == "START")
            {
                
            }
            if (v[0] == "STATUS")
            {
                _statcmd(vec);
            }
            if (v[0] == "RUN")
            {
                WorkerJob::isStopped = false;
                int n = toInt(v[1]);
                WorkerJob::leftItera = n;
                WorkerJob::iterCount = n;
                WorkerJob::isRunning.wakeAll();
            }
            if (v[0] == "EXIT")
            {
                _excmd(vec);
                return;
            }
            if (v[0] == "STOP")
            {
                _stopcmd(vec);
            }*/
        }
    }
    
private:
    
  //  ExitCommand _excmd;
    
   // StatusCommand _statcmd;
    
  //  StopCommand _stopcmd;
    
    void parseCommand(const string& s)
    {
        auto v = split(s);
        string cmd = v[0];
        auto it = commandMap.find(cmd);// check if found
        (*(it->second))(v);
    }
    
    map<string, Command*> commandMap;
    
};



#endif
