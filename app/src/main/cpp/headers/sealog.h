#pragma once

#include <string>
#include <sstream>
#include <android/log.h>

struct _JNIEnv;

#define SEAL_TAG "Grey Seal Engine (Native)"
#define Seal_Log(...) __android_log_print(ANDROID_LOG_INFO, SEAL_TAG, __VA_ARGS__)
#define Seal_LogError(...) __android_log_print(ANDROID_LOG_ERROR, SEAL_TAG, __VA_ARGS__)

typedef std::string Seal_String;
typedef const char* Seal_C_String;

typedef unsigned char Seal_Byte;

inline Seal_String asSealStr(Seal_C_String str) { return Seal_String (str); }
inline Seal_C_String asCStr (const Seal_String& str)  { return str.c_str(); }
_JNIEnv* Seal_JNIEnv(void);
void Seal_NewThreadJNIEnv(_JNIEnv*);