
#include "greyseal/texture.h"

#include "jseal.h"
#include "greyseal/font.h"

#define POS(x, y) ((x) + (y) * w)
#define ALPHA(x) ((x) & 0xFF)
struct __CharacterDetail {
    int start, charStart, end;
};

static std::unordered_map<Seal_FontIndex, Seal_Font*> fonts;
static Seal_FontIndex currentFree = 0;

static const char* _FontCharset = "1 2 3 4 5 6 7 8 9 0 - = q  w e r t y u i o p [ ] a s d f g h j k l ; ' \\\\ z x c v b n m , . / ~ ! @ # $ % ^ & * ( ) _ + Q W E R T Y U I O P  { } A S D F G H J K L : | Z X C V B N M < > ? \\\"";
static const int _FontCharsetLength = strlen(_FontCharset);

static Seal_Font& allocFont(Seal_FontIndex* index, int glId){
    memcpy(index, &currentFree, sizeof(Seal_FontIndex));
    currentFree++;
    Seal_Font& font = *(fonts[*index] = new Seal_Font(glId));
    return font;
}

static __CharacterDetail scanCharacter(Seal_Byte* _t, int w, int h, int& x, int maxRead){
    Seal_Int* texture = (Seal_Int*)_t;
    auto detail = __CharacterDetail();
    int _start = detail.start = x;
    while(x - _start < maxRead){
        int _c = 0;
        for(int y = 0; y < h; y++)
            if(ALPHA(texture[POS(x, y)]) != 0) _c++;
        x++;
        if(_c > 0) break;      // Start of character
    }
    detail.charStart = x;
    while(x - _start < maxRead){
        int _c = 0;
        for(int y = 0; y < h; y++)
            if(ALPHA(texture[POS(x, y)]) != 0) _c++;
        x++;
        if(_c == 0) break;     // End of character
    }
    detail.end = x;

    return detail;
}

Seal_Font::Seal_Font(int glId) : glId(glId), map() {}
Seal_Font::~Seal_Font() {}

extern "C" {
    JNI_FNC(jstring) Java_Link(android_SealTexturePipeline_syncCharset)(JNIEnv *e, jclass) {
        jstring jStr = e->NewStringUTF(_FontCharset);
        return jStr;
    }

    JNI_FNC(jint) Java_Link(android_SealTexturePipeline_registerFont)(JNIEnv *env, jclass,
                                                                      jbyteArray raw,
                                                                      jint w, jint h, jfloat size) {
        int len = w * h * 4;
        Seal_Byte* texture = new Seal_Byte[len];
        int glId = Seal_LoadTextureFromJava(env, raw, w, h, texture);

        if(glId < 0) {
            return SEAL_NO_FONT;
        }

        int x = 0, spaceJump = h / 2;

        Seal_FontIndex index;
        Seal_Font& font = allocFont(&index, glId);

        for(int i = 0; i < _FontCharsetLength; i++){
            if(_FontCharset[i] == ' ') continue;

            __CharacterDetail pair = scanCharacter(texture, w, h, x, 2 * size);

            int width = pair.end - pair.charStart;
            if(_FontCharset[i] == '\"') {
                x = pair.end;
                pair = scanCharacter(texture, w, h, x, size);
                width += pair.end - pair.start;
            }
            x = pair.end;

            if(pair.start != pair.charStart) {
                Seal_UITransform rect = {(float) pair.charStart / w, 0, (float) pair.end / w, 1};
                font.add(_FontCharset[i], rect);
            }
        }
        Seal_Log("Successfully loaded font, %d characters (%d expected)", font.charCount(), _FontCharsetLength / 2);
        return index;
    }
}

Seal_Font* Seal_GetFont(Seal_FontIndex index){
    if(fonts.end() == fonts.find(index)) return nullptr;
    return fonts[index];
}



