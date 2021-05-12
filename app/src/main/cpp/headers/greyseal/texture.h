#pragma once

#include <jni.h>
#include <string>
#include <GLES2/gl2.h>
#include "sealog.h"

typedef int Seal_Texture;

#define SEAL_NO_TEXTURE 0
/**
 * \brief Loads a texture from the the texture cache, or loads a texture from the cache if it fails
 * \param path of the texture
 * \return the GL index of the loaded texture
 */
Seal_Texture Seal_LoadTexture(const Seal_String& path);
void Seal_LoadTextureAsync(const Seal_String& path, Seal_Texture* out);     // TODO: Not working yet
Seal_Texture Seal_LoadWhiteTexture(void);
int Seal_LoadTextureFromJava(JNIEnv* env, jbyteArray raw, jint w, jint h, void* output, bool invert = true);