
#include <jni.h>
#include <GLES2/gl2.h>
#include <unordered_map>

#include "greyseal/threading.h"
#include "greyseal/texture.h"
#include "sealog.h"
#include "jseal.h"

#include "debug/clock.h"

#define P2C(x, y, w) (x) + (w) * (y)
// Reverse the y of a value
#define I_FX(i, w, h) P2C((i) % (w), (h) - (i) / (w) - 1, w)

static std::unordered_map<std::string, Seal_Texture> textures;
static Seal_Mutex jvmMutex;

static Seal_Texture Seal_LoadTextureAndSync(const Seal_String& path, JNIEnv* env){
    if(env == NULL){
        Seal_LogError("Failed to attach JVM");
        return SEAL_NO_TEXTURE;
    }
    // Read the texture file from the java and pass it back here
    Seal_Log("Preloaders: %s", path.c_str());
    jclass texturePipelineJClass = env->FindClass("com/roncho/greyseal/engine/android/SealTexturePipeline");
    Seal_Log("Finding methods...");
    jmethodID textureLoadMethod = env->GetStaticMethodID(texturePipelineJClass, "load", "(Ljava/lang/String;)I");
    Seal_Log("Making jpath");
    jstring jpath = env->NewStringUTF(path.c_str());
    Seal_Log("Making some ids... %p->CallStaticIntMethod(%p,%p,%p)", env, texturePipelineJClass, textureLoadMethod, jpath);
    int glId = env->CallStaticIntMethod(texturePipelineJClass, textureLoadMethod,
                                                  jpath);
    Seal_Log("Caching");
    textures[path] = (Seal_Texture) glId;
    return (Seal_Texture)glId;
}

static void Seal_LoadTextureAsyncHandler(void* args){
    // TODO: The thread's JNIEnv seems not to be synced with the main thread's JNIEnv
    // TODO: Also, attach to thread doesn't fail, but seems like the class loader is not working
    // TODO: properly
    JVM& jvm = Seal_GetJVM();
    // Lock the current thread to make disable race conditions, In addition since we will
    // need to use JNI to register textures, we have to attach the thread.
    jvmMutex.lock();
    JNIEnv* env = jvm.attachToThread();
    Seal_Texture texture = Seal_LoadTextureAndSync(((char*)args + sizeof(Seal_Texture *)), env);
    Seal_Texture* out = *((Seal_Texture**)args);
    // If we want to write this texture somewhere
    if(out)
        *out = texture;
    // Now that we are done with this thread, we can detach the thread and unlock the mutex
    jvm.detachFromThread();
    jvmMutex.unlock();
}

void Seal_LoadTextureAsync(const Seal_String& path, Seal_Texture* out){
    // TODO: Look whats in Seal_LoadTextureAsyncHandler todo
    if(textures.find(path) == textures.end()) {
        // If we haven't loaded the texture yet, then start a thread to load it.
        // Otherwise read it from the cache
        char *args = (char *) malloc(path.size() + 1 + sizeof(Seal_Texture *));
        *((Seal_Texture **) args) = out;
        memcpy(args + sizeof(Seal_Texture *), path.c_str(), path.size());
        args[path.size() + sizeof(Seal_Texture *)] = 0;
        Seal_EnqueueTask(&Seal_LoadTextureAsyncHandler, args);
    }else if(out)
        *out = textures[path];
}

Seal_Texture Seal_LoadTexture(const Seal_String& path){
    if(textures.find(path) == textures.end()) {
        return Seal_LoadTextureAndSync(path, Seal_JNIEnv());
    }
    return textures[path];
}

static void SealTexture_FlipWorkerHandler(void* _args){
    // Flip worker handler function for flipping the X's on an image from Java
    // Used to split the work when there is a lot of it.
    uint32_t* in;
    int w, h, si, ei;
    // Read arguments from argument array
    uint8_t * args = (uint8_t *)_args;
    in = *((uint32_t**)args);
    w = *(int*)(args + sizeof(uint32_t*));
    h = *(int*)(args + sizeof(uint32_t*) + sizeof(int));
    si = *(int*)(args + sizeof(uint32_t*) + 2 * sizeof(int));
    ei = *(int*)(args + sizeof(uint32_t*) + 3 * sizeof(int));
    // Do flipping
    for(int i = si; i < ei; i++){
        int ii = I_FX(i, w, h), temp = in[ii];
        in[ii] = in[i];
        in[i] = temp;
    }
}

int Seal_LoadTextureFromJava(JNIEnv* env, jbyteArray a, jint width, jint height, void* imageArray, bool invert){
    union {
        int8_t* array0;
        uint32_t* array1;
    } buffer;

    // Transfer the array into a c++ array
    size_t arrayLength = env->GetArrayLength(a);
    buffer.array0 = env->GetByteArrayElements(a, JNI_FALSE);

    // First we need to reverse the the array
    // We must invert the Y component of the image for the uvs to mapped correctly
    // Here we decide if we need to split the work to multiple threads or keep it under one thread
    int amount  = (width * height) >> 1;
    int threadCount = 1;
    if(amount > 500000) threadCount = 4;
    int step = amount / threadCount;

    Seal_DebugClock timer;
    timer.start();
    // Check if we need to split the work to threads, to make it faster
    switch (threadCount) {
        case 1:
            // No threads needed, the overhead of making a thread will hurt performance
            for (int i = 0; i < amount; i++) {
                int ii = I_FX(i, width, height), temp = buffer.array1[i];
                buffer.array1[i] = buffer.array1[ii];
                buffer.array1[ii] = temp;
            }
            break;
        default:
            // Initialize the requested threads
            for(int i = 0, si = 0; i < threadCount; i++,si+=step){
                int ei = i == threadCount - 1 ? amount : si + step;
                // Initialize arguments for the threads
                uint8_t* args = (uint8_t *)malloc(sizeof(uint32_t*) + 4 * sizeof(int));
                *((uint32_t**)args) = buffer.array1;
                *(int*)(args + sizeof(uint32_t*)) = width;
                *(int*)(args + sizeof(uint32_t*) + sizeof(int)) = height;
                *(int*)(args + sizeof(uint32_t*) + 2 * sizeof(int)) = si;
                *(int*)(args + sizeof(uint32_t*) + 3 * sizeof(int)) = ei;
                Seal_EnqueueTask(&SealTexture_FlipWorkerHandler, args);
            }
            // Wait for the threads to finish thier work
            Seal_TasksWait();
            break;
    }


    // Now we can do the GL stuff
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint textureId;
    glGenTextures(1, &textureId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.array0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // If a return copy us requested give it
    if(imageArray){
        memcpy(imageArray, buffer.array0, arrayLength);
    }

    timer.stop();
    Seal_Log("Loaded texture in %f seconds (With %d Threads)", timer.elapsedSeconds(), threadCount);

    return textureId;
}

Seal_Texture Seal_LoadWhiteTexture(void){
    GLuint tid;
    glEnable(GL_TEXTURE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &tid);

    Seal_Byte _white[4] = {255, 255, 255, 255};

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, _white);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    textures["textures/white"] = (Seal_Texture)tid;

    return tid;
}

extern "C" {
    JNIEXPORT jint JNICALL Java_com_roncho_greyseal_engine_android_SealTexturePipeline_registerBmp(JNIEnv* env, jclass, jbyteArray a, jint width, jint height){
       return Seal_LoadTextureFromJava(env, a, width, height, nullptr);
    }
}

