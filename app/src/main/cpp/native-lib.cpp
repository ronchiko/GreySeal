#include <jni.h>
#include <string>

#include "sealog.h"
#include "gs_android.h"

#include "greyseal/gl.h"

#include "greyseal/vector3.h"
#include "greyseal/texture.h"
#include "greyseal/material.h"

#define JNI_FNC(ret) JNIEXPORT ret JNICALL

static constexpr int ONE = 1;

extern "C" {
    JNI_FNC(void) Java_com_roncho_greyseal_engine_SealEngineActivity_startEngine(JNIEnv* env, jclass, jobject assetManager){
        Seal_Log("Starting Engine");
        Seal_InitAssets(env, assetManager);
        Seal_Log("Engine Started");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_SealEngineActivity_stopEngine(JNIEnv* env, jclass){
        Seal_FreeMaterials();
        Seal_GlEnd();
        Seal_Log("Closed engine");
    }

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_SealRenderer_init(JNIEnv* env, jclass, jint width, jint height){
        Seal_Log("Starting open GL");
        Seal_InitTexturePipeline(env);
        Seal_GlStart(width, height);
        Seal_Log("Open GL started successfully");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_SealRenderer_step(JNIEnv* e, jclass, jbyteArray update, jbyteArray commands){
        Seal_Byte* sealUpdate = (Seal_Byte *)e->GetByteArrayElements(update, JNI_FALSE);
        Seal_Byte* sealCommands = (Seal_Byte*)e->GetByteArrayElements(commands, JNI_FALSE);
        Seal_Render(sealUpdate, sealCommands);
    }

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_SealSurfaceView_sendSealTouchEvent(JNIEnv*, jobject, jfloat x, jfloat y){

    }

    JNI_FNC(jint) Java_com_roncho_greyseal_engine_systems_stream_SealObjectStream_getSizeofNativeObject(JNIEnv*, jclass){
        return sizeof(Seal_Object);
    }
    JNI_FNC(jint) Java_com_roncho_greyseal_engine_systems_stream_SealEntity_getObjectPayloadSize(JNIEnv*, jclass){
        return 0;   // Currently there is not a feature for individual payloads
    }

    JNI_FNC(jbyteArray) Java_com_roncho_greyseal_engine_android_cpp_SealCppHandler_JNIGet1(JNIEnv* e, jclass){
        jbyte* content = (jbyte*)&ONE;
        jbyteArray jArray = e->NewByteArray(sizeof(int));
        e->SetByteArrayRegion(jArray, 0, sizeof(int), content);
        return jArray;
    }

    JNI_FNC(jbyteArray) Java_com_roncho_greyseal_engine_android_SealRenderer_requestEngineData(JNIEnv* e, jclass){
        size_t bytesSize = Seal_CurrentScene()->bytes();
        jbyteArray array = e->NewByteArray(bytesSize);
        jbyte * jArray = (jbyte*)(Seal_CurrentScene()->byteArray());
        e->SetByteArrayRegion(array, 0, bytesSize, jArray);
        return array;
    }
}