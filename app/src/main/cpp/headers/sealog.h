#pragma once

#include <string>
#include <sstream>
#include <android/log.h>

#define SEAL_TAG "Grey Seal Engine (Native)"
#define Seal_Log(...) __android_log_print(ANDROID_LOG_INFO, SEAL_TAG, __VA_ARGS__)
#define Seal_LogError(...) __android_log_print(ANDROID_LOG_ERROR, SEAL_TAG, __VA_ARGS__)

typedef std::string Seal_String;
typedef const char* Seal_C_String;

typedef int Seal_Bool;
typedef uint8_t Seal_Byte;

typedef int32_t Seal_Int;
typedef int16_t Seal_Short;
typedef float Seal_Float;
typedef int Seal_Size;

#define SEAL_TRUE 1
#define SEAL_FALSE 0

#define SEAL_SUCCESS 0
#define SEAL_FAILURE -1

constexpr inline uint32_t Seal_HashString(Seal_C_String str){
    return !*str ? 5381 : (Seal_HashString(str + 1) * 33) ^ *str;
}
constexpr inline uint64_t Seal_HashString(const Seal_String& str){
    int h = 0;
    for(char c : str)
        h = h * 31 + c;
    return h;
}