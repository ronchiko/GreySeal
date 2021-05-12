
#include <string>
#include <sstream>
#include <unordered_map>

#include "greyseal/material.h"
#include "greyseal/gl.h"

#include "sealog.h"

static std::unordered_map<Seal_String, Seal_MaterialHandle> loaded;
static std::unordered_map<Seal_MaterialHandle, Seal_Material*> materials;
static Seal_MaterialHandle materialIndex = 0;

GLint Seal_Material::find(const char *name) {
    std::string _name = name;
    if(handles.find(_name) == handles.end()){
        glUseProgram(program);
        GLint handle = glGetUniformLocation(program, name);
        handles[_name] = handle;
        return handle;
    }
    return handles[_name];
}

inline static Seal_String Seal_MakeMaterialUid(const Seal_String& vertex, const Seal_String& fragment){
    std::stringstream ss;
    ss << vertex << ";" << fragment << ";";
    return ss.str();
}

Seal_MaterialHandle Seal_LoadMaterial(const Seal_String& vertex, const Seal_String& frag){

    Seal_String pid = Seal_MakeMaterialUid(vertex, frag);
    if(loaded.find(std::string(pid)) != loaded.end()) {
        return loaded[pid];
    }

    GLuint program = Seal_CompileProgram(vertex, frag);
    if(program > 0){
        auto* material = new Seal_Material();
        material->program = program;

        material->vertexHandle = glGetAttribLocation(program, "vertex");
        material->uvHandle = glGetAttribLocation(program, "_uv");
        material->normalHandle = glGetAttribLocation(program, "_normal");

        material->tintHandle = glGetUniformLocation(program, "tint");
        material->textureHandle = glGetUniformLocation(program, "texture");
        material->projectionMatrixHandle = glGetUniformLocation(program, "projectionMatrix");
        material->modelMatrixHandle = glGetUniformLocation(program, "modelMatrix");

        Seal_MaterialHandle id = materialIndex++;
        materials[id] = material;
        loaded[pid] = id;
        return id;
    }
    return -1;
}

Seal_Material* Seal_GetMaterial(Seal_MaterialHandle index){
    if(materials.find(index) == materials.end()) return nullptr;
    return materials[index];
}

void Seal_FreeMaterials(){
    for(auto& pair : materials)
        delete pair.second;
    materials.clear();
}
