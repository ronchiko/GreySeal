#include <string>

#include "sealog.h"
#include "gs_android.h"

#include "greyseal/gl.h"

#include "greyseal/vector3.h"
#include "greyseal/texture.h"
#include "greyseal/material.h"
#include "greyseal/preset.h"
#include "greyseal/calls.h"

#include "jseal.h"

static constexpr int ONE = 1;
static bool __SEAL_INITIALIZED = false;

static float cameraTransformArray[7];

extern "C" {
    JNI_FNC(void) Java_com_roncho_greyseal_engine_SealEngineActivity_startEngine(JNIEnv* env, jclass, jobject assetManager){
        Seal_Log("Starting Engine");
        Seal_InitAssets(env, assetManager);
        Seal_Log("Engine Started");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_SealEngineActivity_stopEngine(JNIEnv* env, jclass){
        Seal_FreeMaterials();
        Seal_EndNttEnvironment();
        Seal_GlEnd();
        Seal_Log("Closed engine");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_SealEngineActivity_preloadCamera(JNIEnv* e, jclass, jfloatArray transform){
        // Length is always 7
        float *elements = e->GetFloatArrayElements(transform, JNI_FALSE);

        memcpy(cameraTransformArray, elements, 7 * sizeof(float));
        std::stringstream  ss;
        for(int i = 0; i < 7; i++){
            ss << cameraTransformArray[i] << ", ";
        }
        Seal_Log("Camera: %s", ss.str().c_str());
    }

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_SealRenderer_init(JNIEnv* env, jclass, jint width, jint height, jbyteArray instr){
        Seal_Log("Starting open GL");
        if(!__SEAL_INITIALIZED) {
            Seal_InitTexturePipeline(env);
            Seal_SetupNttEnvironment();
            Seal_GlStart(width, height);
            Seal_Byte *sealCommands = (Seal_Byte *) env->GetByteArrayElements(instr, JNI_FALSE);
            size_t length = env->GetArrayLength(instr);
            Seal_ExecuteEngineCalls(sealCommands, length);
            Seal_SetCamera(cameraTransformArray);
            __SEAL_INITIALIZED = true;
        }
        Seal_Log("Open GL started successfully");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_SealRenderer_step(JNIEnv* e, jclass, jbyteArray update, jbyteArray commands){
        if(__SEAL_INITIALIZED) {
            Seal_Byte *sealUpdate = (Seal_Byte *) e->GetByteArrayElements(update, JNI_FALSE);
            Seal_Byte *sealCommands = (Seal_Byte *) e->GetByteArrayElements(commands, JNI_FALSE);
            size_t commandsLength = e->GetArrayLength(commands);
            Seal_Render(sealUpdate, sealCommands, commandsLength);
        }
    }

    JNI_FNC(jint) Java_com_roncho_greyseal_engine_systems_stream_EntityStream_getSizeofNativeObject(JNIEnv*, jclass){
        return sizeof(Seal_Entity);
    }
    JNI_FNC(jint) Java_com_roncho_greyseal_engine_systems_stream_Entity_getObjectPayloadSize(JNIEnv*, jclass){
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
        jbyte * jArray = (jbyte*)(Seal_CurrentScene()->bytesArray());
        e->SetByteArrayRegion(array, 0, bytesSize, jArray);

        return array;
    }

    // TODO: When the engine is done, maybe take look and try to optimize with caching
    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addTexture(JNIEnv* env, jclass cls, jstring str, jint index){
        jfieldID fid = env->GetStaticFieldID(cls, "textures", "Ljava/util/HashMap;");
        jobject hashMap = env->GetStaticObjectField(cls, fid);
        jclass cls_hashMap = env->GetObjectClass(hashMap);
        jmethodID mid = env->GetMethodID(cls_hashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        jclass cls_Int = env->FindClass("java/lang/Integer");
        jmethodID mid_IntInit = env->GetMethodID(cls_Int, "<init>", "(I)V");
        jobject o_index = env->NewObject(cls_Int, mid_IntInit, index);

        env->CallObjectMethod(hashMap, mid, str, o_index);
    }

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addMesh(JNIEnv* env, jclass cls, jstring str, jint index){
        jfieldID fid = env->GetStaticFieldID(cls, "meshes", "Ljava/util/HashMap;");
        jobject hashMap = env->GetStaticObjectField(cls, fid);
        jclass cls_hashMap = env->GetObjectClass(hashMap);
        jmethodID mid = env->GetMethodID(cls_hashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        jclass cls_Int = env->FindClass("java/lang/Integer");
        jmethodID mid_IntInit = env->GetMethodID(cls_Int, "<init>", "(I)V");
        jobject o_index = env->NewObject(cls_Int, mid_IntInit, index);

        env->CallObjectMethod(hashMap, mid, str, o_index);
    }

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addMaterial(JNIEnv* env, jclass cls, jstring str, jint index){
        jfieldID fid = env->GetStaticFieldID(cls, "materials", "Ljava/util/HashMap;");
        jobject hashMap = env->GetStaticObjectField(cls, fid);
        jclass cls_hashMap = env->GetObjectClass(hashMap);
        jmethodID mid = env->GetMethodID(cls_hashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        jclass cls_Int = env->FindClass("java/lang/Integer");
        jmethodID mid_IntInit = env->GetMethodID(cls_Int, "<init>", "(I)V");
        jobject o_index = env->NewObject(cls_Int, mid_IntInit, index);

        env->CallObjectMethod(hashMap, mid, str, o_index);
    }
}