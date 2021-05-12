#pragma once

#include "material.h"
#include "color.h"

union Seal_Rect {
public:
    float rect[4];
    struct {
        float x, y;
        float w, h;
    };

    Seal_Rect() : x(0), y(0), w(100), h(100) {}
    Seal_Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
    Seal_Rect(const Seal_Rect& c) : x(c.x), y(c.y), w(c.w), h(c.h) {}

    float endx(void) const { return x + w; }
    float endy(void) const { return y + h; }

    std::string c_str(void) const {
        std::stringstream ss;
        ss << "(" << x << ","  << y << ", " << w << ", " << h << ")";
        return ss.str();
    }
};

typedef int Seal_FontIndex;

/**
 * Class that represents a UI object, which is an object that will be drawn on top the 3D scene
 * The object are controlled through the Seal Engine Java Interface (SEJI) with the UI class.
 */
struct Seal_UIO {
public:
    int flags;
    Seal_Color color, textColor;
    Seal_String text;
    Seal_FontIndex font;
    Seal_Texture texture;
    Seal_Float textSize;
    Seal_MaterialHandle material, textMaterial;
    Seal_Rect transform, padding;

    Seal_UIO();
    ~Seal_UIO();
    /** Draws the UI object */
    void render(void) const;
};

void Seal_AddUIO(int index);
void Seal_SetUIOProp(int, int, float);
void Seal_SetUIOProp(int, int, int);
void Seal_SetUIOProp(int, int, const char*);

void Seal_SetUIOVMode(int, int);        // Render as text or as image
void Seal_DeleteUIO(int);


void Seal_DrawUI(void);