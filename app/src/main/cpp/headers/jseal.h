#pragma once

#include <jni.h>
#include "sealog.h"

/**
 *
 * Handles linking between Java and the C++ with JNI
 *
 */

#define JNI_FNC(ret) JNIEXPORT ret JNICALL

JNIEnv* Seal_JNIEnv(void);

extern "C" {
JNI_FNC(void) Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addTexture(JNIEnv* env, jclass cls, jstring str, jint index);
JNI_FNC(void) Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addMesh(JNIEnv* env, jclass cls, jstring str, jint index);
JNI_FNC(void) Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addMaterial(JNIEnv* env, jclass cls, jstring str, jint index);
}

class Seal_JNIString {
public:
    Seal_JNIString(jstring str) : _host(Seal_JNIEnv()), _jstr(str),
        _chars(_host->GetStringUTFChars(str, JNI_FALSE)), _length(_host->GetStringLength(str)){}
    Seal_JNIString(Seal_C_String str): _host(Seal_JNIEnv()), _jstr(_host->NewStringUTF(str)),
        _chars(_host->GetStringUTFChars(_jstr, JNI_FALSE)), _length(strlen(str)){
    }
    Seal_JNIString(Seal_String str): Seal_JNIString(str.c_str()){

    }

    ~Seal_JNIString(){
        _host->ReleaseStringUTFChars(_jstr, _chars);
        _jstr = nullptr;
    }

    inline size_t length(void) { return _length; }
    char operator[](int index) {
        if(index < 0 || index >= _length) return '\0';
        return _chars[index];
    }

    inline Seal_JNIString copy(void){
        return _host->NewStringUTF(_chars);
    }
private:
    JNIEnv* _host;
    jstring _jstr;
    const char* _chars;
    size_t _length;
};

void Seal_NewThreadJNIEnv(JNIEnv* env);