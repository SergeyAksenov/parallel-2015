#include "Thread.hpp"
#include <iostream>
using namespace std;

map<pthread_t,unsigned> Thread::_numbers;


Thread::Thread (Callable& job): _job(job)
{
    //pthread_t id;
    pthread_create(&_id, NULL, threadRunner, this);
    //_id = id;
}


unsigned Thread::getID ()
{
    return _numbers.find(_id)->second;
    return 0;
}


void* Thread::threadRunner (void* thisThread)
{
    static_cast<Thread*>(thisThread)->_job();
    return NULL;
}


Mutex::Mutex ()
{
    pthread_mutex_t d = PTHREAD_MUTEX_INITIALIZER;
    _descriptor = d;
}


void Mutex::lock ()
{
    pthread_mutex_lock(&_descriptor);
}

void Mutex::unlock ()
{
    pthread_mutex_unlock(&_descriptor);
}


ConditionalVariable::ConditionalVariable ()
{
    pthread_cond_t d = PTHREAD_COND_INITIALIZER;
    _descriptor = d;
}

void ConditionalVariable::wait (Mutex& m)
{
    pthread_cond_wait(&_descriptor, &m._descriptor);
}

void ConditionalVariable::wakeOne ()
{
    pthread_cond_signal(&_descriptor);
}

void ConditionalVariable::wakeAll ()
{
    pthread_cond_broadcast(&_descriptor);
}