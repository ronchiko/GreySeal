#pragma once

#include "material.h"
#include "color.h"

union Seal_UITransform {
public:
    float rect[4];
    struct {
        float x, y;
        float w, h;
    };

    Seal_UITransform() : x(0), y(0), w(100), h(100) {}
    Seal_UITransform(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    float endx(void) const { return x + w; }
    float endy(void) const { return y + h; }
};

typedef int Seal_FontIndex;

struct Seal_UIO {
public:
    int flags;
    Seal_Color color;
    Seal_String text;
    Seal_FontIndex font;
    Seal_Texture texture;
    Seal_MaterialHandle material;
    Seal_UITransform transform;

    Seal_UIO();
    ~Seal_UIO();
    void render(void) const;
};

void Seal_AddUIO(int index);
void Seal_SetUIOProp(int, int, float);
void Seal_SetUIOProp(int, int, int);
void Seal_SetUIOProp(int, int, const char*);

void Seal_SetUIOVMode(int, int);        // Render as text or as image
void Seal_DeleteUIO(int);


void Seal_DrawUI(void);