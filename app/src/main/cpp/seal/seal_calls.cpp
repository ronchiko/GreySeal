
#include "greyseal/calls.h"
#include "greyseal/gl.h"
#include "greyseal/scene.h"
#include "greyseal/preset.h"
#include "greyseal/ui.h"
#include "greyseal/font.h"
#include "greyseal/threading.h"
#include "jseal.h"


#define INST(name) static int SealInst_##name(Seal_ByteStream& stream)

#define READ(read_expr, msg) if((read_expr) != SEAL_SUCCESS) { \
        errorMsg = msg;    \
        return SEAL_FAILURE; \
        }

typedef int (*_Seal_Instruction)(Seal_ByteStream& stream);
static Seal_String errorMsg;

static int Seal_StreamDoCommand(Seal_ByteStream& stream);

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
    Seal_C_String preset = nullptr;
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

    Seal_Log("Preloading texture '%s'", name);
    Seal_LoadTexture(name);

    return SEAL_SUCCESS;
}
INST(LoadMaterial){
    Seal_C_String vertex, fragment;

    READ(stream.readString(&vertex), "Expected a string.");
    READ(stream.readString(&fragment), "Expected a string.");

    Seal_MaterialHandle mtrl = Seal_LoadMaterial(vertex, fragment);

    return SEAL_SUCCESS;
}
INST(LoadMesh){
    Seal_C_String path;

    READ(stream.readString(&path), "Expected a string.");

    int mesh = Seal_LoadMesh(path);
    return SEAL_SUCCESS;
}

INST(For){
    Seal_Byte times;
    READ(stream.readByte(&times), "Expected a byte");

    size_t lpstrt = stream.getPointer();
    for(Seal_Byte b = 0; b < times; b++) {
        if(Seal_StreamDoCommand(stream) == SEAL_FAILURE) return SEAL_FAILURE;
        if (b != times - 1) stream.setPointer(lpstrt);
    }

    return SEAL_SUCCESS;
}

INST(NewUIO){
    Seal_Int index;
    READ(stream.readInt(&index), "Expected an int");
    Seal_AddUIO(index);
    return SEAL_SUCCESS;
}

INST(UIOPropI){
    Seal_Int index, propId, value;
    READ(stream.readInt(&index), "Expected an int");
    READ(stream.readInt(&propId), "Expected an int");
    READ(stream.readInt(&value), "Expected an int");

    Seal_SetUIOProp(index, propId, value);
    return SEAL_SUCCESS;
}
INST(UIOPropF){
    Seal_Int index, propId;
    Seal_Float value;
    READ(stream.readInt(&index), "Expected an int");
    READ(stream.readInt(&propId), "Expected an int");
    READ(stream.readFloat(&value), "Expected a float");

    Seal_SetUIOProp(index, propId, value);
    return SEAL_SUCCESS;
}
INST(UIOPropS){
    Seal_Int index, propId;
    Seal_C_String value;
    READ(stream.readInt(&index), "Expected an int");
    READ(stream.readInt(&propId), "Expected an int");
    READ(stream.readString(&value), "Expected a string");

    Seal_SetUIOProp(index, propId, value);
    return SEAL_SUCCESS;
}

INST(DelUIO){
    Seal_Int index;
    READ(stream.readInt(&index), "Expected an int");
    Seal_DeleteUIO(index);
    return SEAL_SUCCESS;
}

INST(Clone1){
    Seal_Short uid;
    READ(stream.readShort(&uid), "Expected a short");

    Seal_Clone(Seal_Find(uid));

    return SEAL_SUCCESS;
}
INST(Clone2){
    Seal_Short uid;
    Seal_Vector3 ps, sc;
    Seal_Quaternion qt;
    READ(stream.readShort(&uid), "Expected a short");
    READ(stream.readVector3(&ps), "Expected a vector 3");
    READ(stream.readQuaternion(&qt), "Expected a quaternion");
    READ(stream.readVector3(&sc), "Expected a vector 3");

    Seal_Entity* e = Seal_Clone(Seal_Find(uid));
    if(e) {
        e->transform.position = ps;
        e->transform.rotation = qt;
        e->transform.scale = sc;
    }
    return SEAL_SUCCESS;
}

INST(Clone3){
    Seal_Short uid;
    Seal_Vector3 ps;
    READ(stream.readShort(&uid), "Expected a short");
    READ(stream.readVector3(&ps), "Expected a vector 3");

    Seal_Entity* t = Seal_Find(uid);
    Seal_Entity* e = Seal_Clone(t);
    if(e)
        e->transform.position = ps;

    return SEAL_SUCCESS;
}


static _Seal_Instruction instructions[] = {
       &SealInst_Nop,
       &SealInst_Instatiate0,
       &SealInst_Instatiate1,
       &SealInst_Instatiate2,
       &SealInst_Destroy,
       &SealInst_LoadTexture,
       &SealInst_LoadMaterial,
       &SealInst_LoadMesh,
       &SealInst_For,
       &SealInst_Clone1,
       &SealInst_Clone2,
       &SealInst_Clone3,
       &SealInst_NewUIO,
       &SealInst_UIOPropI,
       &SealInst_UIOPropF,
       &SealInst_UIOPropS,
       NULL,
       &SealInst_DelUIO,
       NULL
};

#define SEAL_FAILURE_FATAL -2
static int Seal_StreamDoCommand(Seal_ByteStream& stream){
    // Read opcode from stream
    Seal_Byte opcode;

    if(stream.readByte(&opcode) == SEAL_FAILURE){
        Seal_LogError("Failed to read opcode from instructions.");
        return SEAL_FAILURE_FATAL;
    }
    //Seal_Log("Reading instuction: 0x%X", opcode);

    // Check if we got an illegal opcode
    if(SEAL_INST_NOP > opcode || opcode >= __SEAL_OPCODE_MAX__){
        Seal_LogError("Invalid opcode 0x%X.", (int)opcode);
        return SEAL_FAILURE;
    }

    _Seal_Instruction instruction = instructions[opcode - SEAL_INST_NOP];
    if(instruction(stream) != SEAL_SUCCESS){
        Seal_LogError("Error executing instruction 0x%X: %s.", opcode, errorMsg.c_str());
        return SEAL_FAILURE;
    }
    //Seal_Log("Executed instuction: 0x%X", opcode);
    return SEAL_SUCCESS;
}

void Seal_ExecuteEngineCalls(Seal_Byte* buffer, size_t length){
    Seal_ByteStream stream(buffer, length);
    while(stream.hasNext()){
        if(Seal_StreamDoCommand(stream) == SEAL_FAILURE_FATAL) return;
    }
    Seal_TasksWait();
}
