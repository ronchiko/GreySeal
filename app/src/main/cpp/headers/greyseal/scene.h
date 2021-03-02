#pragma once

#include <vector>
#include <queue>

#include <stdlib.h>
#include <string.h>

#include "object.h"

constexpr float FOV = 45.f, Z_FAR = 100.f, Z_NEAR = .1f;

class Seal_Camera {
public:
    Seal_Camera(const Seal_Vector3& position) : transform(), matrix(new float[16]) {
        transform.position = position;
    }
    ~Seal_Camera()
    {
        delete [] matrix;
    }

    void generateMatrix();

    inline const float* getMatrix() const { return matrix;}

    Seal_Transform transform;
private:
    float* matrix;
};

union Seal_ObjectUnion {
    Seal_Entity object;
    Seal_Byte bytes[sizeof(Seal_Entity)];

    Seal_ObjectUnion() : object() {}
    Seal_ObjectUnion(Seal_ObjectUnion& o) : object(){
        std::memcpy(bytes, o.bytes, sizeof(Seal_Entity));
    }
    Seal_ObjectUnion(Seal_Entity* o) : object(){
        memcpy(bytes, o, sizeof(Seal_Byte));
    }
};

/**
 * \brief Represents a scene
 */
class Seal_Scene {
public:
    Seal_Camera& camera;

    Seal_Scene() : root(), camera(*new Seal_Camera({0, 0, 0})) {
        camera.generateMatrix();
    };
    ~Seal_Scene();

    void drawScene();
    void addObject(Seal_Entity** object);

    inline Seal_Byte* byteArray() { return (Seal_Byte*)root; }

    inline size_t bytes(void) const { return objects * sizeof(Seal_Entity); }
    inline Seal_Entity* getObject(int index) { return (Seal_Entity*)&root[index]; }
    friend void Seal_RenderObject(Seal_Entity *object, Seal_Scene *scene, float *parent);
    friend void Seal_PopDestroy(void);
    friend void Seal_Render(Seal_Byte* updatedData, Seal_Byte* commands, size_t);
    friend void Seal_Instantiate(Seal_Entity** object);
    friend Seal_Entity* Seal_Find(Seal_Short);
private:
    size_t objects;
    Seal_ObjectUnion* root;   // The root object of the scene
};

/**
 * \brief Deletes a object from the scene
 * \param object the object to delete
 */
void Seal_Destroy(Seal_Entity** object);
void Seal_PopDestroy(void);
Seal_Entity* Seal_Find(Seal_Short entityId);
/**
 * \brief Creates a new object and sets its transform
 * \param object The pointer to the new object
 * \param position The position of new object
 */
void Seal_Instantiate(Seal_Entity** object);