#pragma once

#define SEAL_COLOR_MAX 255
#define SEAL_COLOR_MIN 0

constexpr float OneOver255 = 1 / 255.0f;

struct Seal_Color {
    int rgba;

    Seal_Color() : Seal_Color(SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MAX) {}
    Seal_Color(Seal_Byte r, Seal_Byte  g, Seal_Byte  b, Seal_Byte a) : rgba(r << 24 | g << 16 | b << 8 | a) {}
};

inline Seal_Color Seal_RGBA(float r, float g, float b, float a) { return Seal_Color(r, g, b, a); }
inline Seal_Color Seal_RGB(float r, float g, float b) { return Seal_RGBA(r, g, b, SEAL_COLOR_MAX); }
inline bool Seal_CompareRGB(const Seal_Color& a, const Seal_Color& b){
    return a.rgba == b.rgba;
}

#define SEAL_WHITE          Seal_RGB(SEAL_COLOR_MAX, SEAL_COLOR_MAX, SEAL_COLOR_MAX)
#define SEAL_BLACK          Seal_RGB(SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MIN)
#define SEAL_RED            Seal_RGB(SEAL_COLOR_MAX, SEAL_COLOR_MIN, SEAL_COLOR_MIN)
#define SEAL_GREEN          Seal_RGB(SEAL_COLOR_MIN, SEAL_COLOR_MAX, SEAL_COLOR_MIN)
#define SEAL_BLUE           Seal_RGB(SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MAX)
#define SEAL_TRANSPARENT    Seal_RGBA(SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MIN)

