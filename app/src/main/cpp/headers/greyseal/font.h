#pragma once

#include <unordered_map>

#include "jseal.h"
#include "ui.h"
#include "texture.h"

#define SEAL_NO_FONT -1

struct Seal_GlyphMetadata {
    float x, w;
    float wInPixels;
};

class Seal_Font {
public:
    Seal_Font(int glId, int w);
    ~Seal_Font();

    inline void add(char c, float x, float tw, int wp) { map[c] = {x / tw, wp / tw, wp / 48.0f}; }
    inline bool has(char c) const { return map.find(c) != map.end(); }
    inline const Seal_GlyphMetadata& get(char c) { return map[c]; }

    inline int charCount(void) const { return map.size(); }
    inline int getTexture(void) const { return glId; }
    inline int textureWidth(void) const { return w; }
private:
    int w;
    int glId;
    std::unordered_map<char, Seal_GlyphMetadata> map;
};

extern "C" {
    JNI_FNC(jstring) Java_Link(andorid_SealTexturePipeline_syncCharset)(JNIEnv*, jclass);
    JNI_FNC(jint) Java_Link(android_SealTexturePipeline_registerFont)(JNIEnv*, jclass, jbyteArray,
            jint w, jint h, jfloat size);
}

Seal_Font* Seal_GetFont(Seal_FontIndex);

