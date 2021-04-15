#pragma once

#include <unordered_map>

#include "jseal.h"
#include "ui.h"
#include "texture.h"

#define SEAL_NO_FONT -1

class Seal_Font {
public:
    Seal_Font(int glId);
    ~Seal_Font();

    inline void add(char c, Seal_UITransform t) { map[c] = t; }
    inline bool has(char c) const { return map.find(c) != map.end(); }
    inline auto get(char c) { return map[c]; }

    inline int charCount(void) const { return map.size(); }
private:
    int glId;
    std::unordered_map<char, Seal_UITransform> map;
};

extern "C" {
    JNI_FNC(jstring) Java_Link(andorid_SealTexturePipeline_syncCharset)(JNIEnv*, jclass);
    JNI_FNC(jint) Java_Link(android_SealTexturePipeline_registerFont)(JNIEnv*, jclass, jbyteArray,
            jint w, jint h, jfloat size);
}

Seal_Font* Seal_GetFont(Seal_FontIndex);

