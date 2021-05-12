
#include "greyseal/texture.h"

#include "jseal.h"
#include "greyseal/font.h"

#define POS(x, y) ((x) + (y) * w)
#define ALPHA(x) ((x) & 0xFF)

struct _ImageBuffer {
    int* buffer;
    int w, h;

    _ImageBuffer(int* buffer, int w, int h) : buffer(buffer), w(w), h(h) {}

    inline bool transparentAt(int x, int y) const { return !(buffer[x + y * w] & 0xFF); }

    /// Scans the line for non-transparent pixels
    inline int scanLine(int x) const{
        int trp = 0;
        for(int i = 0; i < h; i++) if(!transparentAt(x, i)) trp++;
        return trp;
    }
};

static std::unordered_map<Seal_FontIndex, Seal_Font*> fonts;
static Seal_FontIndex currentFree = 0;

static std::string _FontCharset = "1 2 3 4 5 6 7 8 9 0 - = q  w e r t y u i o p [ ] a s d f g h j k l ; ' \\ z x c v b n m , . / ~ ! @ # $ % & * ( ) _ + Q W E R T Y U I O P { } A S D F G H J K L : | Z X C V B N M < > ? ^ ` \"";

Seal_Font::Seal_Font(int glId, int w) : w(w), glId(glId), map() {}
Seal_Font::~Seal_Font() {}

static void SealFont_ScanCharacter(int start, int& final, int& width, const _ImageBuffer& image){
    int sc = 0, x = start;
    while(!sc) sc = image.scanLine(x++);
    int mx = x;
    while(sc) sc = image.scanLine(x++);
    final = x;
    width = final - mx;
}

extern "C" {
    JNI_FNC(jstring) Java_Link(android_SealTexturePipeline_syncCharset)(JNIEnv *e, jclass) {
        jstring jStr = e->NewStringUTF(_FontCharset.c_str());
        return jStr;
    }

    JNI_FNC(jint) Java_Link(android_SealTexturePipeline_registerFont)(JNIEnv *env, jclass,
                                                                      jbyteArray raw,
                                                                      jint w, jint h, jfloat size) {
        int len = w * h * 4;
        Seal_Byte* texture = new Seal_Byte[len];
        int glId = Seal_LoadTextureFromJava(env, raw, w, h, texture, false);

        _ImageBuffer image = _ImageBuffer((int*)texture, w, h);



        Seal_FontIndex index = currentFree++;
        Seal_Font* font = fonts[index] = new Seal_Font(glId, w);

        int x = 0, width = 0;
        for(char& c_ : _FontCharset){
            if(c_ == ' ') continue;

            int sx = x;
            SealFont_ScanCharacter(sx, x, width, image);
            font->add(c_, x - width, w, width);
        }

        Seal_Log("Successfully loaded font %d", index);
        delete[] texture;
        return index;
    }
}

Seal_Font* Seal_GetFont(Seal_FontIndex index){
    if(fonts.end() == fonts.find(index)) return nullptr;
    return fonts[index];
}



