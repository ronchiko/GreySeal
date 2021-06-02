#pragma once

#include <vector>
#include <queue>

#include <stdlib.h>
#include <string.h>

#include "object.h"

constexpr float FOV = 45.f, Z_FAR = 100.f, Z_NEAR = .1f;

union Seal_ObjectUnion {
    Seal_Entity object;
    Seal_Byte bytes[sizeof(Seal_Entity)];

    Seal_ObjectUnion() : object() {}
    Seal_ObjectUnion(Seal_ObjectUnion& o) : object(){
        std::memcpy(bytes, o.bytes, sizeof(Seal_Entity));
    }
    Seal_ObjectUnion(Seal_Entity* o) : object(){
        memcpy(bytes, o, sizeof(Seal_Entity));
    }

    Seal_Entity* operator->(void) { return &object; }
};

/**
 * \brief Represents a scene
 *
 *
 */
class Seal_Scene {
public:
    Seal_Scene() : count(1), start((Seal_ObjectUnion*)malloc(sizeof(Seal_ObjectUnion))) {
        Seal_Entity e = Seal_Entity();
        e.engineFlags = SEAL_ENGINE_DONT_DRAW;
        e.uid = 0;
        start[0] = Seal_ObjectUnion(&e);
    }
    ~Seal_Scene() { free(start); }

    inline size_t bytes(void) const { return count * sizeof(Seal_ObjectUnion); }
    inline Seal_Byte* bytesArray(void) const { return (Seal_Byte *)start; }
    /** \brief Remove all objects marked for destruction */
    void cleanse(void);
    void render(void) const;
    void push(Seal_Entity* entities, int count);
    inline void push(Seal_Entity& entity) { push(&entity, 1); }

    inline Seal_Entity& camera() const { return start->object; }
private:
    inline Seal_ObjectUnion* entities(void) const { return start + 1; }
    void viewMatrix(float*) const;          // create a projection matrix

    friend void Seal_Render(Seal_Byte* updatedBytes, Seal_Byte* calls, size_t callArrayLength);
    friend Seal_Entity* Seal_Find(Seal_Short id);

    int count;
    Seal_ObjectUnion* start;   // The root object of the scene
};

/**
 * \brief Deletes a object from the scene
 * \param object the object to delete
 */
void Seal_Destroy(Seal_Entity** object);
Seal_Entity* Seal_Find(Seal_Short entityId);
Seal_Entity* Seal_Clone(Seal_Entity* other);
/**
 * \brief Sets the camera transform, where transform vector is an array of length 7 that has the position and rotation
 * @param transformVector
 */
void Seal_SetCamera(float* transformVector);
/**
 * \brief Creates a new object and sets its transform
 * \param object The pointer to the new object
 * \param position The position of new object
 */
void Seal_Instantiate(Seal_Entity** object);