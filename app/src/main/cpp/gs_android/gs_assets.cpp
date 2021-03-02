#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "sealog.h"
#include "gs_android.h"


typedef AAssetManager* Manager;

static Manager manager;

int Seal_InitAssets(JNIEnv* env, jobject _manager){
    manager = AAssetManager_fromJava(env, _manager);

    return 1;
}

int Seal_LoadTxt(Seal_String path, char** buffer) {
    Seal_C_String cPath =  path.c_str();
    AAsset* asset = AAssetManager_open(manager, cPath, AASSET_MODE_STREAMING);

    if(!asset) {
        Seal_LogError("No asset named %s.", cPath);
        return -1;
    }

    off_t length = AAsset_getLength(asset);
    *buffer = new char[length + 1];
    int len = AAsset_read(asset, *buffer, length);
    (*buffer)[length] = '\0';
    AAsset_close(asset);

    return len;
}