#pragma once

#include <jni.h>
#include <string>
#include <GLES2/gl2.h>
#include "sealog.h"

typedef int Seal_Texture;

#define SEAL_NO_TEXTURE 0

/**
 * \brief Initialize the texture loading pipeline
 * \param e The jni env
 * \return 1 if successful 0 if failed.
 */
int Seal_InitTexturePipeline(JNIEnv* e);
/**
 * \brief Loads a texture from the the texture cache, or loads a texture from the cache if it fails
 * \param path of the texture
 * \return the GL index of the loaded texture
 */
Seal_Texture Seal_LoadTexture(const Seal_String& path);