
#include <jni.h>

#include "jseal.h"
#include "greyseal/scene.h"
#include "greyseal/gl.h"

static JNIEnv* runningEnv;
static JVM* jvm;

JNIEnv* Seal_JNIEnv(void) { return runningEnv; }
void Seal_NewThreadJNIEnv(JNIEnv* env) { runningEnv = env; }

void Seal_CreateJVM(JNIEnv* env){
    jvm = new JVM(env);
}

void Seal_DestroyJVM(void){
    delete jvm;
}
JVM& Seal_GetJVM(void){
    return *jvm;
}

JNIEnv * JVM::attachToThread(void) {
    JNIEnv* _env = NULL;

    if(jvm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_OK){
        return _env;
    }

    JavaVMAttachArgs vmAttachArgs;
    vmAttachArgs.version = JNI_VERSION_1_6;
    vmAttachArgs.name = NULL;
    vmAttachArgs.group = NULL;

    int result = jvm->AttachCurrentThread(&_env, &vmAttachArgs);
    if(result == JNI_OK) {
        return _env;
    }
    Seal_LogError("Failed to attach thread to current program");
    return NULL;
}
void JVM::detachFromThread() {
    jvm->DetachCurrentThread();
}

JVM::JVM(JNIEnv* e) : jvm(NULL), env(e){
    e->GetJavaVM(&jvm);
}
