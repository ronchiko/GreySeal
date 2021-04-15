
#include <string>
#include <seal.ntt.h>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <stdio.h>

#include "greyseal/gl.h"
#include "greyseal/object.h"
#include "greyseal/preset.h"
#include "greyseal/mesh.h"
#include "greyseal/material.h"
#include "greyseal/texture.h"
#include "gs_android.h"

#define VECTOR3_F "v3"
#define QUATERNION_F "qt"
#define EULER_F "euler"
#define LOAD_F "path"
#define SHADER_F "shader"
#define RGBA_F "rgba"
#define RGB_F "rgb"

#define V3_T Vector3NttlType
#define QT_T QuaternionNttlType
#define FILE_T FileNttlType
#define GLS_T ShaderNttlType

#define CLEANUP cleanup

#define VERIFY(cnd) if(!(cnd)) { SealNtt_RaiseError(NTTE_InvalidArgument, SEAL_NTT_FUNCTION); return SEAL_NTT_ERROR; }
#define OVRD_IF(f, x, _) if(preset->_preset & f) (x)->_ = preset->_
#define PF_ON(f) out->_preset = (Seal_PresetFlags)(out->_preset | f)
#define NTT_FNC_(name) static int name (SealNtt_Object* out, SealNtt_Object* argv, size_t argc)

enum Seal_PresetFlags : uint16_t {
    PF_None = 0,

    PF_POSITION = 0x1,
    PF_ROTATION = 0x2,
    PF_SCALE = 0x4,
    PF_FLAGS = 0x8,
    PF_TEXTURE = 0x10,
    PF_MESH = 0x20,
    PF_MATERIAL = 0x40,
    PF_COLOR = 0x80,
};

struct Seal_ObjectPreset {
public:
    Seal_ObjectPreset() :
        _preset(PF_None), flags(0), position({0, 0, 0}), scale({1,1,1}), rotation({1,0,0,0}),
        color(SEAL_WHITE.rgba),
        material(SEAL_NO_MATERIAL), texture(SEAL_NO_TEXTURE), mesh(SEAL_NO_MESH) {}
private:
    Seal_PresetFlags _preset;       // Contains the info about which fields can be overridden while writing to an object

    int flags;
    Seal_Vector3 position, scale;
    Seal_Quaternion rotation;

    int color;
    Seal_MaterialHandle material;
    Seal_Texture texture;
    int mesh;

    friend int HandleNttProperty(char* property, const SealNtt_Object* value, void* o);
    friend int Seal_OverwriteObject(Seal_Entity* o, Seal_Preset presetIndex, Seal_Bool alsoPosition);
};

static std::unordered_map<Seal_String, Seal_Preset> pathToIndex;
static std::unordered_map<Seal_Preset, Seal_ObjectPreset*> presets;
static Seal_Preset __CurrentPreset = 0;
static int Vector3NttlType, QuaternionNttlType, FileNttlType, ShaderNttlType;

NTT_FNC_(SealNtt_V3Func){
    int result = SEAL_NTT_SUCCESS;

    if(argv[0].type != SEAL_NTT_NUM || argv[1].type != SEAL_NTT_NUM || argv[2].type != SEAL_NTT_NUM){
        SealNtt_RaiseError(NTTE_InvalidArgument, SEAL_NTT_FUNCTION);
        return SEAL_NTT_ERROR;
    }

    Seal_Vector3 *value = (Seal_Vector3 *) malloc(sizeof(Seal_Vector3));
    SEAL_NTT_SAFE(SealNtt_StringToNum((Seal_C_String)argv[0].data, &value->x));
    SEAL_NTT_SAFE(SealNtt_StringToNum((Seal_C_String)argv[1].data, &value->y));
    SEAL_NTT_SAFE(SealNtt_StringToNum((Seal_C_String)argv[2].data, &value->z));

    out->data = value;
    out->type = V3_T;
CLEANUP:
    return result;
}

NTT_FNC_(SealNtt_EulerFunc){
    int result = SEAL_NTT_SUCCESS;

    if(argv[0].type != SEAL_NTT_NUM || argv[1].type != SEAL_NTT_NUM || argv[2].type != SEAL_NTT_NUM){
        SealNtt_RaiseError(NTTE_InvalidArgument, SEAL_NTT_FUNCTION);
        return SEAL_NTT_ERROR;
    }

    Seal_Vector3 value = {0, 0, 0};
    SEAL_NTT_SAFE(SealNtt_StringToNum((Seal_C_String)argv[0].data, &value.x));
    SEAL_NTT_SAFE(SealNtt_StringToNum((Seal_C_String)argv[1].data, &value.y));
    SEAL_NTT_SAFE(SealNtt_StringToNum((Seal_C_String)argv[2].data, &value.z));

    Seal_Quaternion q = Seal_Quaternion::euler(value.x , value.y, value.z);
    out->data = malloc(sizeof(Seal_Quaternion));
    std::memcpy(out->data, &q, sizeof(Seal_Quaternion));
    out->type = QT_T;

CLEANUP:
    return result;
}

NTT_FNC_(SealNtt_QtFunc) {
    int result = SEAL_NTT_SUCCESS;

    for(int i = 0; i < argc; i++) {
        if (argv[i].type != SEAL_NTT_NUM) {
            SealNtt_RaiseError(NTTE_InvalidArgument, SEAL_NTT_FUNCTION);
            return SEAL_NTT_ERROR;
        }
    }

    Seal_Quaternion* value = (Seal_Quaternion*)malloc(sizeof(Seal_Quaternion));
    SEAL_NTT_SAFE(SealNtt_StringToNum((const char*)argv[0].data, &value->w));
    SEAL_NTT_SAFE(SealNtt_StringToNum((const char*)argv[1].data, &value->x));
    SEAL_NTT_SAFE(SealNtt_StringToNum((const char*)argv[2].data, &value->y));
    SEAL_NTT_SAFE(SealNtt_StringToNum((const char*)argv[3].data, &value->z));

    out->data = value;
    out->type = QT_T;
CLEANUP:
    return result;
}

NTT_FNC_(SealNtt_FileFunc) {
    int result = SEAL_NTT_SUCCESS;

    if(argv[0].type != SEAL_NTT_STR) {
        SealNtt_RaiseError(NTTE_InvalidArgument, SEAL_NTT_FUNCTION);
        return SEAL_NTT_ERROR;
    }

    out->type = FileNttlType;
    out->data = strdup((Seal_C_String)argv[0].data);

    return result;
}

NTT_FNC_(SealNtt_ShaderFunc) {
    int result = SEAL_NTT_SUCCESS;

    if(argv[0].type != SEAL_NTT_STR || argv[1].type != SEAL_NTT_STR){
        SealNtt_RaiseError(NTTE_InvalidArgument, SEAL_NTT_FUNCTION);
        return SEAL_NTT_ERROR;
    }

    int shader = Seal_LoadMaterial((const char*)argv[0].data, (const char*)argv[1].data);
    if(shader == SEAL_NO_MATERIAL){
        char buffer[500];
        sprintf(buffer, "Failed to initialize shader with '%s' & '%s'", argv[0].data, argv[1].data);
        SealNtt_RaiseError(NTTE_Custom, buffer);
        result = SEAL_NTT_ERROR;
        goto cleanup;
    }
    out->type = GLS_T;
    *(int*)(out->data = malloc(sizeof(int))) = shader;
cleanup:
    return result;
}

NTT_FNC_(SealNtt_RgbaFunc) {
    for(int i = 0; i < argc; i++) {
        if (argv[i].type != SEAL_NTT_NUM) {
            SealNtt_RaiseError(NTTE_InvalidArgument, SEAL_NTT_FUNCTION);
            return SEAL_NTT_ERROR;
        }
    }

    float rgba[4];
    for(int i = 0; i < argc; i++) {
        SEAL_NTT_SAFE(SealNtt_StringToNum((const char*)argv[i].data, &rgba[i]))
    }

    Seal_Color c = Seal_Color(rgba[0], rgba[1], rgba[2], rgba[3]);
    char buffer[20];
    snprintf(buffer, 20, "%d", c.rgba);
    out->data = strdup(buffer);
    out->type = SEAL_NTT_NUM;

    return SEAL_NTT_SUCCESS;
}

NTT_FNC_(SealNtt_RgbFunc) {
    for(int i = 0; i < argc; i++) {
        if (argv[i].type != SEAL_NTT_NUM) {
            SealNtt_RaiseError(NTTE_InvalidArgument, SEAL_NTT_FUNCTION);
            return SEAL_NTT_ERROR;
        }
    }

    float rgba[3];
    for(int i = 0; i < argc; i++) {
        SEAL_NTT_SAFE(SealNtt_StringToNum((const char*)argv[i].data, &rgba[i]))
    }

    Seal_Color c = Seal_Color(rgba[0], rgba[1], rgba[2], 1);
    char buffer[20];
    snprintf(buffer, 20, "%d", c.rgba);
    out->data = strdup(buffer);
    out->type = SEAL_NTT_NUM;

    return SEAL_NTT_SUCCESS;
}

inline void HandleNttError(const char* msg){
    if(msg) Seal_LogError("Seal-Ntt Error: %s", msg);
}

int Seal_SetupNttEnvironment(void){
    SealNtt_Init(SEAL_NTT_MAXFNC, NTTF_DontQuitOnError);

    SealNtt_RegisterErrorCallback(&HandleNttError);

    // Create the Seal NTT Lang types
    V3_T = SealNtt_NewType();
    QT_T = SealNtt_NewType();
    FILE_T = SealNtt_NewType();
    GLS_T = SealNtt_NewType();

    // Register the seal ntt functions
    SEAL_NTT_SAFE(SealNtt_NewFunc(VECTOR3_F, &SealNtt_V3Func, 3));
    SEAL_NTT_SAFE(SealNtt_NewFunc(EULER_F, &SealNtt_EulerFunc, 3));
    SEAL_NTT_SAFE(SealNtt_NewFunc(QUATERNION_F, &SealNtt_QtFunc, 4));
    SEAL_NTT_SAFE(SealNtt_NewFunc(LOAD_F, &SealNtt_FileFunc, 1));
    SEAL_NTT_SAFE(SealNtt_NewFunc(SHADER_F, &SealNtt_ShaderFunc, 2));
    SEAL_NTT_SAFE(SealNtt_NewFunc(RGB_F, &SealNtt_RgbFunc, 3));
    SEAL_NTT_SAFE(SealNtt_NewFunc(RGBA_F, &SealNtt_RgbaFunc, 4));

    return SEAL_SUCCESS;
}

int HandleNttProperty(char* property, const SealNtt_Object* value, void* o){
    Seal_ObjectPreset* out = (Seal_ObjectPreset*)o;

    switch (Seal_HashString(property)) {
        case Seal_HashString("flags"):
            VERIFY(SEAL_NTT_ARG_IS("flags", SEAL_NTT_NUM));
            PF_ON(PF_FLAGS);
            return SealNtt_StringToInt((const char*)value->data, &out->flags);
        case Seal_HashString("model"):
            VERIFY(SEAL_NTT_ARG_IS("model", FILE_T));
            out->mesh = Seal_LoadMesh(Seal_String((char*)value->data));
            PF_ON(PF_MESH);
            return SEAL_NTT_SUCCESS;
        case Seal_HashString("texture"):
            VERIFY(SEAL_NTT_ARG_IS("texture", FILE_T));
            out->texture = Seal_LoadTexture(Seal_String((char*)value->data));
            PF_ON(PF_TEXTURE);
            return SEAL_NTT_SUCCESS;
        case Seal_HashString("position"):
            VERIFY(SEAL_NTT_ARG_IS("position", V3_T));
            out->position = Seal_Vector3(*(Seal_Vector3*)value->data);
            PF_ON(PF_POSITION);
            return SEAL_NTT_SUCCESS;
        case Seal_HashString("scale"):
            VERIFY(SEAL_NTT_ARG_IS("scale", V3_T));
            out->scale = Seal_Vector3(*(Seal_Vector3*)value->data);
            PF_ON(PF_SCALE);
            return SEAL_NTT_SUCCESS;
        case Seal_HashString("rotation"): {
            VERIFY(SEAL_NTT_ARG_IS("rotation", QT_T));
            out->rotation = *(Seal_Quaternion*)value->data;
            PF_ON(PF_ROTATION);
        }return SEAL_NTT_SUCCESS;
        case Seal_HashString("material"):
            VERIFY(SEAL_NTT_ARG_IS("material", GLS_T));
            out->material = *((Seal_MaterialHandle *)value->data);
            PF_ON(PF_MATERIAL);
            return SEAL_NTT_SUCCESS;
        case Seal_HashString("color"):
            VERIFY(SEAL_NTT_ARG_IS("color", SEAL_NTT_NUM));
            if(SealNtt_StringToInt((const char*)value->data, &out->color) == -1)
                return SEAL_NTT_ERROR;
            PF_ON(PF_COLOR);
            return SEAL_NTT_SUCCESS;
    }

failed:
    SealNtt_RaiseError(NTTE_InvalidProperty, property);
    return SEAL_NTT_ERROR;
}

void Seal_EndNttEnvironment(void){
    Seal_PresetWipeCache();
    SealNtt_Cleanup();
}

Seal_Preset Seal_LoadCachePreset(const Seal_String& path){
    if(pathToIndex.find(path) != pathToIndex.end()){    // If we already have this object, we don't need to reinitialize it.
         return pathToIndex[path];
    }

    char* nttFile = NULL;
    if(Seal_LoadTxt(path, &nttFile) == -1) {
        return -1;
    }
    Seal_ObjectPreset* preset = new Seal_ObjectPreset();

    SealNtt_Parse(nttFile, (void*)preset, &HandleNttProperty);
    Seal_Preset index = __CurrentPreset++;

    pathToIndex[path] = index;
    presets[index] = preset;

    delete nttFile;
    return index;
}

void Seal_PresetWipeCache(void) {
    for(auto& item : presets){
        delete item.second;
    }
    presets.clear();
    pathToIndex.clear();
    __CurrentPreset = 0;
}

int Seal_OverwriteObject(Seal_Entity* o, Seal_Preset presetIndex, Seal_Bool alsoPosition){
    if(!o) return SEAL_FAILURE;

    const Seal_ObjectPreset* preset = presets[presetIndex];
    if(!preset) return SEAL_FAILURE;

    if(preset->_preset & PF_FLAGS)
        o->flags = *((Seal_ObjectFlags*)&(preset->flags));

    if(alsoPosition){
        OVRD_IF(PF_POSITION,(&o->transform), position);
    }

    OVRD_IF(PF_ROTATION,(&o->transform), rotation);
    OVRD_IF(PF_SCALE,(&o->transform), scale);

    OVRD_IF(PF_TEXTURE, o, texture);
    OVRD_IF(PF_MATERIAL, o, material);
    OVRD_IF(PF_MESH, o, mesh);

    if(preset->_preset & PF_COLOR) {
        o->color = Seal_Color();
        o->color.rgba = preset->color;
    }

    return SEAL_SUCCESS;
}