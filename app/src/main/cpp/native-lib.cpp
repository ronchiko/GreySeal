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
        Seal_CreateJVM(env);
        Seal_InitAssets(env, assetManager);
        Seal_Log("Engine Started");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_SealEngineActivity_stopEngine(JNIEnv* env, jclass){
        Seal_FreeMaterials();
        Seal_EndNttEnvironment();
        Seal_DestroyJVM();
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

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_SealRenderer_init(JNIEnv* env, jclass){
        Seal_Log("Starting open GL");
        //if(!__SEAL_INITIALIZED) {
        Seal_NewThreadJNIEnv(env);
        Seal_SetupNttEnvironment();
        __SEAL_INITIALIZED = true;
        //}
        Seal_Log("Open GL started successfully");
    }
    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_SealRenderer_updateSpecs(JNIEnv* env, jclass, jint w, jint h, jbyteArray instr){
        Seal_Log("Updating specs %d %d", w,h);

        Seal_GlStart();

        Seal_Specs::width = w;
        Seal_Specs::height = h;
        Seal_Byte *sealCommands = (Seal_Byte *) env->GetByteArrayElements(instr, JNI_FALSE);
        size_t length = env->GetArrayLength(instr);
        Seal_ExecuteEngineCalls(sealCommands, length);
        glViewport(0, 0, Seal_Specs::width, Seal_Specs::height);
        Seal_SetCamera(cameraTransformArray);
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

    JNI_FNC(void) Java_com_roncho_greyseal_engine_android_cpp_SealLinkedCache_addMesh(JNIEnv* env, jclass cls, jstring str, jint index){

        jclass cacheClass = env->FindClass("com/roncho/greyseal/engine/android/cpp/SealLinkedCache");
        jmethodID include = env->GetStaticMethodID(cacheClass, "includeMesh", "(Ljava/lang/String;I)V");
        env->CallStaticVoidMethod(cacheClass, include, str, index);

        Seal_Mesh* mesh = Seal_GetMesh(index);
        if(mesh && mesh->size() > 0){
            // Pre-compute the AABB of this mesh
            // AABB is calculated relative to the object's center by finding the minimum and maximum
            // X, Y, Z values of the mesh.
            const Seal_Vector3* vertecies = reinterpret_cast<const Seal_Vector3*>(mesh->getVertecies());
            Seal_Vector3 min(vertecies->x, vertecies->y, vertecies->z), max(min.x, min.y, min.z);
            for(int i = 1; i < mesh->size(); i++){
                const Seal_Vector3& vertex = vertecies[i];
                if(min.x > vertex.x) min.x = vertex.x;
                if(min.y > vertex.y) min.y = vertex.y;
                if(min.z > vertex.z) min.z = vertex.z;
                if(max.x < vertex.x) max.x = vertex.x;
                if(max.y < vertex.y) max.y = vertex.y;
                if(max.z < vertex.z) max.z = vertex.z;
            }
            // The size of the AABB is the max vector - the min vector
            Seal_Vector3 sizes = max - min;
            // Register the AABB on the Java interface
            jclass aabb = env->FindClass("com/roncho/greyseal/engine/physics/AABB");
            jmethodID registerMethod = env->GetStaticMethodID(aabb, "registerMeshAABB", "(IFFFFFF)V");
            env->CallStaticVoidMethod(aabb, registerMethod, index, min.x, min.y, min.z,
                    sizes.x, sizes.y, sizes.z);
        }
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

    JNI_FNC(jbyteArray) Java_Link(android_SealSurfaceView_makeSnapshot)(JNIEnv* env, jclass){

        Seal_Scene* scene = Seal_CurrentScene();
        size_t size = scene->bytes() + sizeof(Seal_Scene);

        jbyteArray array = env->NewByteArray(size);
        env->SetByteArrayRegion(array, 0, sizeof(Seal_Scene), (jbyte*)scene);
        env->SetByteArrayRegion(array, sizeof(Seal_Scene), scene->bytes(), (jbyte*)scene->bytesArray());

        return array;
    }
}