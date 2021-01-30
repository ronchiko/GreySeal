#pragma once

#include <vector>

#include "vector3.h"
#include "quaternion.h"
#include "material.h"
#include "texture.h"
#include "mesh.h"
#include "color.h"

typedef unsigned int Seal_ObjectFlags;

class Seal_Scene;

union Seal_Transform {
    struct {
        Seal_Vector3 position;
        Seal_Vector3 scale;
        Seal_Quaternion rotation;
    };
    float raw[10];

    Seal_Transform() : position({0, 0, -10}), scale({1, 1, 1}), rotation({1, 0, 0, 0}) {}

    void move(float x, float y, float z);

    void transformMatrix(float*);
};

struct Seal_Object {
public:
    Seal_Object() : Seal_Object(nullptr)  {}
    Seal_Object(Seal_Object* parent) : material(SEAL_NO_MATERIAL), transform(), mesh(SEAL_NO_MESH), texture(SEAL_NO_TEXTURE), color(SEAL_WHITE), parent(parent), children(), flags(0) {}
    ~Seal_Object();

    inline void setColor(const Seal_Color& c) {color = c; }
    inline void setTexture(Seal_Texture t) { texture = t; }
    void setMesh(int mesh);
    void setMaterial(int);
    void addChild(Seal_Object* o) { children.push_back(o); }
    friend void Seal_RenderObject(Seal_Object* object, Seal_Scene* scene, float* parent);

    Seal_Transform transform;
private:
    Seal_MaterialHandle material;
    int mesh;
    Seal_Texture texture;
    Seal_Color color;
    Seal_Object* parent;
    std::vector<Seal_Object*> children;
    Seal_ObjectFlags flags;

    friend int Seal_Reparent(Seal_Object*, Seal_Object*);
    friend int Seal_Deparent(Seal_Object*);
};

/**
 * \brief renders a seal object to the screen
 */
void Seal_RenderObject(Seal_Object* object, Seal_Scene* scene, float* parentTransform);
/**
 * \brief Deparents a object
 * \param object object to deparent
 */
int Seal_Deparent(Seal_Object* object);