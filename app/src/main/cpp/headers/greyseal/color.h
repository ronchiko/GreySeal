#pragma once

#define SEAL_COLOR_MAX 1
#define SEAL_COLOR_MIN 0

union Seal_Color {
    struct {
        float r, g, b, a;
    };
    float rgba[4];

    Seal_Color() : r(SEAL_COLOR_MIN), g(SEAL_COLOR_MIN), b(SEAL_COLOR_MIN), a(SEAL_COLOR_MAX) {}
    Seal_Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    inline bool operator==(const Seal_Color& c) const { return r == c.r && b == c.b && g == c.g && a == c.a; }
};

inline Seal_Color Seal_RGBA(float r, float g, float b, float a) { return {r, g, b, a}; }
inline Seal_Color Seal_RGB(float r, float g, float b) { return {r,g,b, SEAL_COLOR_MAX}; }
inline bool Seal_CompareRGB(const Seal_Color& a, const Seal_Color& b){
    return a.r == b.r && a.b == b.b && a.g == b.g;
}

#define SEAL_WHITE {SEAL_COLOR_MAX, SEAL_COLOR_MAX, SEAL_COLOR_MAX, SEAL_COLOR_MAX}
#define SEAL_BLACK {SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MAX}
#define SEAL_RED   {SEAL_COLOR_MAX, SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MAX}
#define SEAL_GREEN {SEAL_COLOR_MIN, SEAL_COLOR_MAX, SEAL_COLOR_MIN, SEAL_COLOR_MAX}
#define SEAL_BLUE  {SEAL_COLOR_MIN, SEAL_COLOR_MIN, SEAL_COLOR_MAX, SEAL_COLOR_MAX}