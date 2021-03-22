
#include <jni.h>
#include <GLES2/gl2.h>
#include <unordered_map>

#include "greyseal/texture.h"
#include "sealog.h"
#include "jseal.h"

#define P2C(x, y, w) (x) + (w) * (y)
// Reverse the y of a value
#define I_FX(i, w, h) P2C((i) % (w), (h) - (i) / (w), w)

static std::unordered_map<std::string, Seal_Texture> textures;
static jmethodID textureLoadMethod;
static jclass texturePipelineJClass;

int Seal_InitTexturePipeline(JNIEnv* env){
    Seal_Log("Initializing texture pipeline");
    Seal_NewThreadJNIEnv(env);

    texturePipelineJClass = env->FindClass("com/roncho/greyseal/engine/android/SealTexturePipeline");
    if(env->ExceptionCheck()) return JNI_FALSE;
    textureLoadMethod = env->GetStaticMethodID(texturePipelineJClass, "load", "(Ljava/lang/String;)I");

    Seal_Log("Texture pipeline initialized successfully");
    return env->ExceptionCheck();
}

Seal_Texture Seal_LoadTexture(const Seal_String& path){
    if(textures.find(path) == textures.end()) {
        // The the texture isn't loaded, load it
        int glId = Seal_JNIEnv()->CallStaticIntMethod(texturePipelineJClass, textureLoadMethod,
                                                   Seal_JNIEnv()->NewStringUTF(path.c_str()));

        textures[path] = (Seal_Texture) glId;
        return (Seal_Texture)glId;
    }
    return textures[path];
}

extern "C" {
    JNIEXPORT jint JNICALL Java_com_roncho_greyseal_engine_android_SealTexturePipeline_registerBmp(JNIEnv* env, jclass, jbyteArray a, jint width, jint height){
        union {
            int8_t* array0;
            uint32_t* array1;
        } textureArray, buffer;

        // Transfer the array into a c++ array
        size_t arrayLength = env->GetArrayLength(a);
        textureArray.array0 = env->GetByteArrayElements(a, JNI_FALSE);

        // First we need to reverse the the array
        buffer.array0 = new int8_t[arrayLength];
        size_t floatArrayLength = arrayLength / 4;

        // We must invert the X component of the image for the uvs to mapped correctly
        for(int i = 0; i < floatArrayLength; i++)
            buffer.array1[i] = textureArray.array1[I_FX(i, width, height)];

        // Now we can do the GL stuff
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLuint textureId;
        glGenTextures(1, &textureId);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.array0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Free buffer
        delete[] buffer.array0;
        return textureId;
    }
}

