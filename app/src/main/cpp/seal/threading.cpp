#include <stdlib.h>
#include <vector>

#include "greyseal/threading.h"
#include "sealog.h"

typedef void* ptr_t;

template<typename T> inline T cast(void* value){
    return (T)value;
}

Seal_Thread::Seal_Thread(Seal_ThreadHandler handler, Seal_ThreadArgs args) : threadHandle() {
    done = false;
    ptr_t* arguments = (ptr_t*)malloc(3 * sizeof(ptr_t));
    arguments[0] = (void*)handler;
    arguments[1] = (void*)args;
    arguments[2] = (void*)this;

    pthread_create(&threadHandle, NULL, &handle, arguments);
}

void * Seal_Thread::handle(void * args) {
    // Args is an array of pointers, where args[0] is the function address
    // ,args[1] is a pointer to the argument array and args[2] is the thread object
    ptr_t* ptr_args = (ptr_t*)args;
    void (*functionHandler)(void*)  = cast<void (*)(void*)>(ptr_args[0]);
    void* arguments = cast<void*>(ptr_args[1]);
    Seal_Thread* self = cast<Seal_Thread*>(ptr_args[2]);
    // Call the handler method
    (*functionHandler)(arguments);
    // Free our arguments
    free(arguments);
    free(args);
    self->done = true;
    // Exit from thread safely
    pthread_exit(NULL);
}

void Seal_Thread::join(void) const {
    pthread_join(threadHandle, 0);
}

//-------------------------------------------
//          Thread Queue
//-------------------------------------------

class Seal_ThreadQueue {
public:
    Seal_ThreadQueue() : locked(false), threads() {}

    void add(Seal_ThreadHandler handler, Seal_ThreadArgs args){
        if(!locked){

            threads.push_back(new Seal_Thread(handler, args));
        }
    }
    void await(void) {
        locked = true;
        for(auto& thread : threads) thread->join();
    }
    void clear(void){
        for(auto & thread : threads){
            delete thread;
        }
        threads.clear();
        locked = false;
    }
    int threadCount(void) const { return threads.size(); }
private:
    bool locked;
    std::vector<Seal_Thread*> threads;
} threadQueue;

void Seal_EnqueueTask(Seal_ThreadHandler handler, Seal_ThreadArgs args){
    threadQueue.add(handler, args);
}

void Seal_TasksWait(void){
    int log = threadQueue.threadCount();
    threadQueue.await();
    threadQueue.clear();
    if(log > 0) Seal_Log("Done waiting for threads! (%d threads done)", log);
}