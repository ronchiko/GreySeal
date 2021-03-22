#pragma once

#include "sealog.h"

#include "greyseal/vector3.h"
#include "greyseal/quaternion.h"

/**
 * \brief Represents a stream of bytes that can be advanced linearly
 */
class Seal_ByteStream{
public:
    Seal_ByteStream(Seal_Byte*, size_t length);

    int readShort(Seal_Short*);
    int readByte(Seal_Byte*);
    int readInt(Seal_Int*);
    int readFloat(Seal_Float*);
    int readString(Seal_C_String*);
    int readVector3(Seal_Vector3*);
    int readQuaternion(Seal_Quaternion*);

    inline Seal_Bool hasNext(void) { return current < length; }

    inline void setPointer(size_t ind) { current = ind; }
    inline size_t getPointer(void) { return current; }
private:
    Seal_Byte* advance(int);

    Seal_Byte* buffer;
    size_t current, length;
};


typedef enum _Seal_CallType_e : Seal_Byte {
    SEAL_INST_NOP = 0,              // NOP
    SEAL_INST_INSTANTIATE_0,        // SNT str v3 qt v3
    SEAL_INST_INSTANTIATE_1,        // SNT str v3
    SEAL_INST_INSTANTIATE_2,        // SNT str
    SEAL_INST_DESTROY,              // DESTROY oid
    SEAL_INST_LOAD_TEXTURE,         // LDT str
    SEAL_INST_LOAD_MATERIAL,        // LDS str str
    SEAL_INST_LOAD_MESH,            // LDM str
    SEAL_INST_FOR,                  // FOR byte [INSTRUCTION]
    SEAL_INST_CLONE_1,              // CLN oid
    SEAL_INST_CLONE_2,              // CLN oid v3 qt v3
    SEAL_INST_CLONE_3,              // CLN oid v3

    __SEAL_OPCODE_MAX__
} Seal_EngineCallType;

void Seal_ExecuteEngineCalls(Seal_Byte* buffer, size_t length);