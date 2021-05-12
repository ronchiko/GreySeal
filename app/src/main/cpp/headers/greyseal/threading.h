#pragma once

#include <pthread.h>

//--------------------------------------------------------
// PThread Wrapper
//--------------------------------------------------------

typedef void (*Seal_ThreadHandler)(void*);
typedef void* Seal_ThreadArgs;

class Seal_Mutex {
public:
    Seal_Mutex() : mutex() {
        pthread_mutex_init(&mutex, NULL);
    }
    ~Seal_Mutex() {
        pthread_mutex_destroy(&mutex);
    }

    inline void lock(void) { pthread_mutex_lock(&mutex);}
    inline void unlock(void) { pthread_mutex_unlock(&mutex);}
private:
    pthread_mutex_t mutex;
};

class Seal_Thread {
public:
    Seal_Thread(Seal_ThreadHandler, Seal_ThreadArgs = NULL);
    void join(void) const;
private:
    static void* handle(void*);

    bool done;
    pthread_t threadHandle;
};

//--------------------------------------------------------
// Thread Queue
//--------------------------------------------------------

void Seal_EnqueueTask(Seal_ThreadHandler next, Seal_ThreadArgs args);
/** Waits for all the tasks to be done */
void Seal_TasksWait(void);