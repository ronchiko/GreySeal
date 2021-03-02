
#include <jni.h>

#include "jseal.h"

static JNIEnv* runningEnv;

JNIEnv* Seal_JNIEnv(void) { return runningEnv; }
void Seal_NewThreadJNIEnv(JNIEnv* env) { runningEnv = env; }
