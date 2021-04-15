
#include <vector>

#include "greyseal/initialize.h"
#include "greyseal/scene.h"
#include "greyseal/gl.h"

template<typename T> inline T* realloc(void* ptr, size_t size) { return (T*)realloc(ptr, size); }

#define QUEUE_RESIZE 10
static struct {
public:
    Seal_Entity& emplace(void){
        if(count == used){
            queue = realloc<Seal_Entity>(queue, (used + QUEUE_RESIZE) * sizeof(Seal_Entity));
            used += QUEUE_RESIZE;
        }
        return (queue[count++] =  Seal_Entity());
    }

    inline Seal_Entity* data(void) { return queue; }
    inline int size(void) { return count; }

    inline void clear(void){
        count = 0;
    }
private:
    int count = 0, used = 0;
    Seal_Entity *queue = nullptr;
} queue;

Seal_Entity& Seal_QueueLateEntity(void){
    return queue.emplace();
}

void Seal_LateInitialize(void){
    if(queue.size() > 0) {
        Seal_Scene *scene = Seal_CurrentScene();
        scene->push(queue.data(), queue.size());
        queue.clear();
    }
}


