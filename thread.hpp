#ifndef thread_h
#define thread_h

#include <vector>
#include <map>
#include <utility>
#include "ICallable.hpp"
#include <pthread.h>

using namespace std;



class Thread
{
    friend void *threadRunner (void* thisThread);
    

public:
    
    Thread (Callable& job);
    
    unsigned getID ();
    
    void join()
    {
        pthread_join(_id, NULL);
    }
    
private:
    
    static void *threadRunner (void* thisThread);
    
    Thread (const Thread&);
    
    void operator = (const Thread&);
    
    pthread_t _id;
    
    Callable& _job;
    
    static map<pthread_t,unsigned> _numbers;
    
};



class AbstractMutex
{
    
public:
    
    virtual ~AbstractMutex() {}
    
    virtual void lock() = 0;
    
    virtual void unlock() = 0;
    
};



class Mutex : public AbstractMutex
{
    
    friend class ConditionalVariable;
    
public:
    
    Mutex ();
    
    void lock ();
    
    void unlock ();
    
private:
    
    pthread_mutex_t _descriptor;
    
};



class ConditionalVariable
{
    
public:
    
    ConditionalVariable ();
    
    void wait (Mutex& m);
    
    void wakeOne ();
    
    void wakeAll ();
    
private:
    
    pthread_cond_t _descriptor;
    
};


#endif