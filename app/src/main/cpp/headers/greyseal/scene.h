#pragma once

#include <vector>
#include <queue>

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
private:
    Seal_Transform transform;
    float* matrix;
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
    void addObject(Seal_Object* object);

    friend void Seal_RenderObject(Seal_Object *object, Seal_Scene *scene, float *parent);
    friend int Seal_Reparent(Seal_Object*, Seal_Object*);
    friend void Seal_PopDestroy(void);
private:
    std::vector<Seal_Object*> root;   // The root object of the scene
};

/**
 * \brief Moves a seal object from a parent to a different parent, if the parent is null the object will be deleted
 * \param child the object to move
 * \param newParent the parent to transfer the object to
 * \return -1 on failure, 0 otherwise
 */
int Seal_Reparent(Seal_Object* child, Seal_Object* newParent);
/**
 * \brief Deletes a object from the scene
 * \param object the object to delete
 */
void Seal_Destroy(Seal_Object** object);
void Seal_PopDestroy(void);