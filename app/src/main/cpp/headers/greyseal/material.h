#pragma once

#include <GLES2/gl2.h>

#include "texture.h"
#include "sealog.h"

#include <unordered_map>
#include <string>

#define SEAL_NO_MATERIAL -1

typedef int Seal_MaterialHandle;

struct Seal_Material {
public:
    Seal_Material() : program(), vertexHandle(), normalHandle(), projectionMatrixHandle(),
                      modelMatrixHandle(), tintHandle(), textureHandle() {}
    GLint find(const char* name);
public:
    GLuint program;
    GLint vertexHandle, uvHandle, normalHandle;

    GLint projectionMatrixHandle, modelMatrixHandle;
    GLint tintHandle;
    GLint textureHandle;
private:
    std::unordered_map<std::string, GLint> handles;
};

Seal_MaterialHandle Seal_LoadMaterial(const Seal_String& vertex, const Seal_String& fragment);
Seal_Material* Seal_GetMaterial(Seal_MaterialHandle index);
void Seal_FreeMaterials();