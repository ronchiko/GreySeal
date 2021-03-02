
#include "greyseal/calls.h"
#include "greyseal/gl.h"
#include "greyseal/scene.h"
#include "greyseal/preset.h"

#include "jseal.h"

#define INST(name) static int SealInst_##name(Seal_ByteStream& stream)

#define READ(read_expr, msg) if((read_expr) != SEAL_SUCCESS) { \
        errorMsg = msg;    \
        return SEAL_FAILURE; \
        }

typedef int (*_Seal_Instruction)(Seal_ByteStream& stream);
static Seal_String errorMsg;

INST(Nop){
    return SEAL_SUCCESS;
}
//
INST(Instatiate0){
    Seal_C_String preset;
    Seal_Vector3 pos, scale;
    Seal_Quaternion qt;

    READ(stream.readString(&preset), "Expected a string");
    READ(stream.readVector3(&pos), "Expected a vector 3");
    READ(stream.readQuaternion(&qt), "Expected quaternion");
    READ(stream.readVector3(&scale), "Expected a vector 3");

    Seal_Entity* object;
    Seal_Instantiate(&object);
    Seal_OverwriteObject(object, Seal_LoadCachePreset(preset), true);
    object->transform.position = pos;
    object->transform.rotation = qt;
    object->transform.scale = scale;

    return SEAL_SUCCESS;
}
INST(Instatiate1){
    Seal_C_String preset;
    Seal_Vector3 pos;

    READ(stream.readString(&preset), "Expected a string");
    READ(stream.readVector3(&pos), "Expected a vector 3");

    Seal_Entity* object;
    Seal_Instantiate(&object);
    Seal_OverwriteObject(object, Seal_LoadCachePreset(preset), true);
    object->transform.position = pos;

    return SEAL_SUCCESS;
}
INST(Instatiate2){
    Seal_C_String preset;

    READ(stream.readString(&preset), "Expected a string");

    Seal_Entity* object;
    Seal_Instantiate(&object);
    Seal_OverwriteObject(object, Seal_LoadCachePreset(preset), true);

    return SEAL_SUCCESS;
}
// Destroy inst
INST(Destroy){
    Seal_Short entityId;

    READ(stream.readShort(&entityId), "Expected a short.");

    Seal_Entity* ntt = Seal_Find(entityId);
    if(ntt != nullptr) Seal_Destroy(&ntt);

    return SEAL_NTT_SUCCESS;
}
// Loaders
INST(LoadTexture){
    Seal_C_String name;

    READ(stream.readString(&name), "Expected a string.");

    Seal_Texture texture = Seal_LoadTexture(name);
    jclass jcls = Seal_JNIEnv()->FindClass("com/roncho/greyseal/engine/android/cpp/SealLinkedCache");
    jstring str = Seal_JNIEnv()->NewStringUTF(name);
    Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addTexture(Seal_JNIEnv(), jcls, str, texture);
    return SEAL_SUCCESS;
}
INST(LoadMaterial){
    Seal_C_String vertex, fragment;

    READ(stream.readString(&vertex), "Expected a string.");
    READ(stream.readString(&fragment), "Expected a string.");

    Seal_MaterialHandle mtrl = Seal_LoadMaterial(vertex, fragment);
    std::stringstream ss; ss << vertex << ";" << fragment << ";";
    jclass jcls = Seal_JNIEnv()->FindClass("com/roncho/greyseal/engine/android/cpp/SealLinkedCache");
    jstring str = Seal_JNIEnv()->NewStringUTF(ss.str().c_str());
    Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addMaterial(Seal_JNIEnv(), jcls, str, mtrl);
    return SEAL_SUCCESS;
}
INST(LoadMesh){
    Seal_C_String path;

    READ(stream.readString(&path), "Expected a string.");

    int mesh = Seal_LoadMesh(path);
    jclass jcls = Seal_JNIEnv()->FindClass("com/roncho/greyseal/engine/android/cpp/SealLinkedCache");
    jstring str = Seal_JNIEnv()->NewStringUTF(path);
    Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addMesh(Seal_JNIEnv(), jcls, str, mesh);
    return SEAL_SUCCESS;
}

static _Seal_Instruction instructions[] = {
       &SealInst_Nop,
       &SealInst_Instatiate0,
       &SealInst_Instatiate1,
       &SealInst_Instatiate2,
       &SealInst_Destroy,
       &SealInst_LoadTexture,
       &SealInst_LoadMaterial
};

void Seal_ExecuteEngineCalls(Seal_Byte* buffer, size_t length){
    Seal_ByteStream stream(buffer, length);
    while(stream.hasNext()){
        // Read opcode from stream
        Seal_Byte opcode;
        if(stream.readByte(&opcode) == SEAL_FAILURE){
            Seal_LogError("Failed to read opcode from instructions.");
            return;
        }

        // Check if we got an illegal opcode
        if(SEAL_INST_NOP > opcode || opcode >= __SEAL_OPCODE_MAX__){
            Seal_LogError("Invalid opcode 0x%X.", (int)opcode);
            continue;
        }

        _Seal_Instruction instruction = instructions[opcode - SEAL_INST_NOP];
        if(instruction(stream) != SEAL_SUCCESS){
            Seal_LogError("Error executing instruction 0x%X: %s.", opcode, errorMsg.c_str());
            continue;
        }
    }
}
