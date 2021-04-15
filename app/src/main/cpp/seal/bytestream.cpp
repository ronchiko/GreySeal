
#include <cstdint>

#include "greyseal/calls.h"

#define READ_FUNC(name, cast) \
	int Seal_ByteStream::name(cast* value){ \
	    size_t size = sizeof(cast);           \
		Seal_Byte* buf = advance(size);			\
		if(buf){\
			memcpy(value, buf, size);       \
			return SEAL_SUCCESS;	        \
		}				\
		return SEAL_FAILURE;		        \
	}

Seal_ByteStream::Seal_ByteStream(Seal_Byte* buffer, size_t length)
        : buffer(buffer), current(0), length(length){
}

Seal_Byte* Seal_ByteStream::advance(int count) {
    if(current + count > length){
        return nullptr;
    }

    Seal_Byte* addrs = buffer + current;
    current += count;
    return addrs;
}

READ_FUNC(readShort, Seal_Short);
READ_FUNC(readInt, Seal_Int);
READ_FUNC(readByte, Seal_Byte);
READ_FUNC(readFloat, Seal_Float);
READ_FUNC(readQuaternion, Seal_Quaternion);
READ_FUNC(readVector3, Seal_Vector3);

#define NOW count + current

int Seal_ByteStream::readString(Seal_C_String* value){
    int count = 0;
    while(NOW < length && buffer[NOW] != '\0')
        count++;

    *value = nullptr;
    // If the current one isn't zero, then we ran out of space
    if(buffer[NOW] != '\0') return SEAL_FAILURE;
    *value = (char*)(buffer + current);
    advance(count + 1);
    return SEAL_SUCCESS;
}

