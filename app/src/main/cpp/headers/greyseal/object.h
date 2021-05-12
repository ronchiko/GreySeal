#pragma once

#include <vector>

#include "vector3.h"
#include "quaternion.h"
#include "material.h"
#include "texture.h"
#include "mesh.h"
#include "color.h"

typedef unsigned int Seal_ObjectFlags;
typedef Seal_Short Seal_EngineFlags;
class Seal_Scene;

typedef enum _Seal_EngineFlag_e : Seal_EngineFlags {
    SEAL_ENGINE_DESTROY = 0x1,
    SEAL_ENGINE_NEW = 0x2,
    SEAL_ENGINE_DONT_DRAW = 0x4,
    SEAL_ENGINE_DESTROYED = 0x8
} Seal_EngineFlag;

struct Seal_Transform {
    Seal_Vector3 position;
    Seal_Quaternion rotation;
    Seal_Vector3 scale;

    Seal_Transform() : position({0, 0, 0}), rotation({1, 0, 0, 0}), scale({1, 1, 1}) {}

    void move(float x, float y, float z);

    void transformMatrix(float*);
};

struct Seal_Entity {
public:
    Seal_Entity() : flags(0), engineFlags(0), uid(0), transform(), material(SEAL_NO_MATERIAL), mesh(SEAL_NO_MESH), texture(SEAL_NO_TEXTURE), color(SEAL_WHITE) {}

    inline void setColor(const Seal_Color& c) {color = c; }
    inline void setTexture(Seal_Texture t) { texture = t; }
    inline void setMesh(int m) { mesh = m; }
    inline void setMaterial(Seal_MaterialHandle m) { material = m; }

    Seal_ObjectFlags flags;
    Seal_EngineFlags engineFlags;
    uint16_t uid;
    Seal_Transform transform;
    Seal_Color color;
    Seal_MaterialHandle material;
    Seal_Texture texture;
    int mesh;

    friend void Seal_RenderObject(Seal_Entity* object, const Seal_Scene* scene, float* parent, float* view);
};

/**
 * \brief renders a seal object to the screen
 */
void Seal_RenderObject(Seal_Entity* object, const Seal_Scene* scene, float* parentTransform, float* view);

#define Seal_LogObject(o) Seal_Log("P(%f %f %f)-V(%d %d %d)", o->transform.position.x, o->transform.position.y, o->transform.position.z, o->mesh, o->material, o->texture)