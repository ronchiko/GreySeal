#pragma once

#include <jni.h>
#include "sealog.h"

/**
 * \brief initializes the asset manager in c++ code
 *
 * \return 1 on success, 0 on failure
 * */
int Seal_InitAssets(JNIEnv *env, jobject _manager);
/**
 * \brief reads an asset from the asset manager
 *
 * \return the length of the new buffer, -1 if failed to load
 */
int Seal_LoadTxt(Seal_String, char**);