#include <jni.h>
#include <string>

#include "sealog.h"
#include "gs_android.h"

#include "greyseal/gl.h"

#include "greyseal/vector3.h"
#include "greyseal/texture.h"
#include "greyseal/material.h"

#define JNI_FNC(ret) JNIEXPORT ret JNICALL

extern "C" {
    JNI_FNC(void) Java_com_roncho_greyseal_engine_GreySealEngineActivity_startEngine(JNIEnv* env, jclass, jobject assetManager){
        Seal_Log("Starting Engine");
        Seal_InitAssets(env, assetManager);
        Seal_Log("Engine Started");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_GreySealEngineActivity_stopEngine(JNIEnv* env, jclass){
        Seal_FreeMaterials();
        Seal_GlEnd();
        Seal_Log("Closed engine");
    }

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_GreySealRenderer_init(JNIEnv* env, jclass, jint width, jint height){
        Seal_Log("Starting open GL");
        Seal_InitTexturePipeline(env);
        Seal_GlStart(width, height);
        Seal_Log("Open GL started successfully");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_GreySealRenderer_step(JNIEnv*, jclass){
        Seal_Render();
    }

    JNI_FNC(jfloat) Java_com_roncho_greyseal_engine_objects_NativeObject_getFloat(JNIEnv*,jclass, jlong address, jint offset){
        return *(jfloat*)((void*)(address + offset));
    }

    JNI_FNC(jint) Java_com_roncho_greyseal_engine_objects_NativeObject_getInteger(JNIEnv*,jclass, jlong address, jint offset){
        return *(jint*)((void*)(address + offset));
    }

    JNI_FNC(void) Java_com_roncho_greyseal_engine_objects_NativeObject_setInteger(JNIEnv*, jclass, jlong address, jint offset, jint value){
        *(jint*)((void*)(address + offset)) = value;
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_objects_NativeObject_setFloat(JNIEnv*, jclass, jlong address, jint offset, jfloat value){
        *(jfloat*)((void*)(address + offset)) = value;
    }

    JNI_FNC(jlong) Java_com_roncho_greyseal_engine_objects_hooks_Vector3_createNative(JNIEnv*,jclass, jfloat x, jfloat y, jfloat z){
        return (long)((void*)new Seal_Vector3(x, y, z));
    }

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_GreySealSurfaceView_sendSealTouchEvent(JNIEnv*,jclass, jfloat x, jfloat y){

    }
}