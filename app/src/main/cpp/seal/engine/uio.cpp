
#include <unordered_map>
#include <algorithm>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "greyseal/ui.h"
#include "greyseal/gl.h"
#include "greyseal/font.h"

#define MAP_ONE_MINUS_VALUE(value, range) (1 - static_cast<float>(value)/range * 2)
#define MAP_VALUE(value, range) (static_cast<float>(value)/range * 2 - 1)
#define ARR_SET_POINT(x, y, u, v) vertecies[_ptr] = (x); uvs[_ptr++] = (u); vertecies[_ptr] = (y); uvs[_ptr++] = (v);
#define SAFE_SET(index) if(uioArray.find(index) == uioArray.end()) { \
    return;    \
    }
#define OUT(name) &OUT_##name

#define REGION_X(v) ((v) & 0x3)
#define REGION_Y(v) (((v) & 0x3) << 2)

static std::unordered_map<int, Seal_UIO> uioArray;

extern int defaultTextureId;

static float QUAD_UVS[8] =       {  0, 0, 1, 0,  0,  1, 1,  1 };
typedef enum __DrawingFlags {
    Draw = 0x1,
    TextObject = 0x2
} Seal_UioDrawingFlags;

enum TextFlags {
    Axis_Warp = 0x0,
    Axis_Bleed = 0x1,
    Axis_Clamp = 0x2,
    Axis_Peek = 0x3
};
enum AlignmentFlags {
    Align_Left = 0x0,
    Align_Center = 0x1,
    Align_Right = 0x2
};

enum UioPropIndecies {
    I_Texture = 0,
    I_Font = 1,
    I_Material = 2,

    F_PositionX = 3,
    F_PositionY = 4,
    F_Width = 5,
    F_Height = 6,

    I_Color = 7,

    S_Text = 8,

    I_TextMaterial = 9,
    I_TextColor = 10,
    F_TextSize = 11,

    F_PaddingL = 12,
    F_PaddingT = 13,
    F_PaddingR = 14,
    F_PaddingB = 15,

    I_Flags = 16
};

void Seal_DrawUI(void){
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(auto& uio : uioArray)
        uio.second.render();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

Seal_UIO::Seal_UIO() : flags(Align_Left << 2 | Align_Left), color(SEAL_WHITE), textColor(SEAL_BLACK), text(), font(SEAL_NO_FONT),
    texture(SEAL_NO_TEXTURE),
    textSize(48), material(SEAL_NO_MATERIAL), textMaterial(SEAL_NO_MATERIAL), transform() ,padding({5, 5, 5, 5}){
    color = Seal_Color(255, 255, 255, 255);
    transform = { 0, 0, 100, 100 };
    textMaterial = material = Seal_LoadMaterial("vertex-ui.glsl", "fragment-ui.glsl");
    texture = Seal_LoadTexture("textures/white");
}

Seal_UIO::~Seal_UIO() {}

inline static float alignTextOrigin(int flags, float current, float start, float end, int shift = 0){
    flags >>= shift;
    switch(flags & 0x3){
        case Align_Center:
            return start + (end - current) / 2;
        case Align_Right:
            return end + start - current;
        case Align_Left:
        default:
            return start;
    }
}

/**
 * Returns an array of X origins, Y origin and the amount of X origin for the input text.
 * Make sure to call 'delete[]' on the output array.
 * Function will return 'nullptr' if no text can fit inside the given region, in that case no free
 * is needed.
 */
inline static float* preRenderText(const Seal_Rect& region, float size, int flags,
        const std::string& text, float& sy, int& lines, Seal_Font* font){

    std::vector<float> _lines;
    float x = region.x, y = region.y;

    // Check if we can even fit text in the wanted size inside this box
    if(region.y + size >= region.endy()){
        lines = 0;
        sy = region.y;
        return nullptr;
    }

    for(auto& c : text){
        if(font->has(c)){
            auto& glyph = font->get(c);
            int w = glyph.wInPixels * size;
            if(x + w >= region.endx()){
                // If next character we will cross the region boundaries, then we call assume we have
                // a line, so we append it and drop a line
                _lines.push_back(alignTextOrigin(flags, x, region.x, region.endx()));
                x = region.x;
                y += size;
                // Check if we reached the end of the Y region
                if(y + size >= region.endy())
                    break;
            }
            x += w;
        }else{
            // If the character is not recognized, print a space
            x += size * 0.25f;
        }
    }
    // If we didn't quit because of lack of space, then append the last line
    if(y + size < region.endy()) {
        y += size;
        _lines.push_back(alignTextOrigin(flags, x, region.x, region.endx()));
    }

    // After all is well and done, set the output variables, allocate a buffer for the X origin
    // array and align the Y origin. Make sure to call delete[] lines after being done with lines
    lines = _lines.size();
    float* o_lines = new float[lines];
    memcpy(o_lines, _lines.data(), lines * sizeof(float));
    sy = alignTextOrigin(flags, y, region.y, region.endy(), 2);

    return o_lines;
}

void Seal_UIO::render(void) const {
    float _screen[2] = { (float)Seal_Specs::width, (float)Seal_Specs::height };

    // Draw the background image
    if(material > SEAL_NO_MATERIAL && texture > SEAL_NO_TEXTURE && color.a > 0) {
        Seal_Material* _material = Seal_GetMaterial(material);
        glUseProgram(_material->program);

        float _vertecies[8] = {
               transform.x, transform.endy(),
               transform.endx(), transform.endy(),
               transform.x, transform.y,
               transform.endx(), transform.y
        };

        glEnableVertexAttribArray(_material->vertexHandle);
        glEnableVertexAttribArray(_material->uvHandle);

        glVertexAttribPointer(_material->vertexHandle, 2, GL_FLOAT, false, 0, _vertecies);
        glVertexAttribPointer(_material->uvHandle, 2, GL_FLOAT, false, 0, QUAD_UVS);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(_material->textureHandle, 1);

        glUniform2fv(_material->find("screen"), 1, _screen);

        float _color[4] = {
                (color.r) * OneOver255,
                (color.g) * OneOver255,
                (color.b) * OneOver255,
                (color.a) * OneOver255
        };
        glUniform4fv(_material->tintHandle, 1, _color);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glDisableVertexAttribArray(_material->vertexHandle);
        glDisableVertexAttribArray(_material->uvHandle);

        glBindTexture(GL_TEXTURE_2D, SEAL_NO_TEXTURE);
    }

    // Draw text on top of the background
    if(!text.empty() && font > SEAL_NO_FONT && textMaterial > SEAL_NO_MATERIAL){
        std::vector<float> vertecies, uvs;

        // Get objects from handles
        Seal_Material* _material = Seal_GetMaterial(textMaterial);
        Seal_Font* _font = Seal_GetFont(font);

        // Call pre render text, and make sure can we render text on the background
        // If we can't the we can just exit and save the redundent GL calls
        float x = transform.x, y = transform.y, h = textSize;
        int lineCount = 0, currentLine = 0;
        Seal_Rect region = {transform.x + padding.x, transform.y + padding.y, transform.w - padding.w, transform.h - padding.h};
        float* lines = preRenderText(region, textSize, flags, text, y, lineCount, _font);

        if(!lineCount) return;

        // Setup the X origin of the first line, and start drawing text
        x = lines[currentLine++];
        for(const char& c : text){
            if(_font->has(c)){
                auto& glyph = _font->get(c);
                float w = glyph.wInPixels * textSize;

                if(x + w > region.endx()){
                    // If we reach the end of a line, then check if we have a next line
                    // If not we break, otherwise we drop a line
                    if(currentLine >= lineCount)
                        break;

                    x = lines[currentLine++];
                    y += textSize;
                }

                // Cull text if its not visible (Outside of the screen)
                if(x > Seal_Specs::width || x + w < 0){
                    x += w;
                    continue;
                }

                // Push vertecies into arrays
                // TODO: Maybe try to pull a cleaner solution?
                vertecies.push_back(x); vertecies.push_back(y);
                vertecies.push_back(x + w); vertecies.push_back(y + h);
                vertecies.push_back(x + w); vertecies.push_back(y);
                vertecies.push_back(x); vertecies.push_back(y);
                vertecies.push_back(x); vertecies.push_back(y + h);
                vertecies.push_back(x + w); vertecies.push_back(y + h);

                uvs.push_back(glyph.x); uvs.push_back(1);
                uvs.push_back(glyph.x + glyph.w); uvs.push_back(0);
                uvs.push_back(glyph.x + glyph.w); uvs.push_back(1);
                uvs.push_back(glyph.x); uvs.push_back(1);
                uvs.push_back(glyph.x); uvs.push_back(0);
                uvs.push_back(glyph.x + glyph.w); uvs.push_back(0);

                x += w;
            }else{
                // If the character is not defined in the font, the print a space
                x += textSize / 4;
            }
        }
        delete[] lines;

        // Do GL draw call
        glUseProgram(_material->program);

        glUniform2fv(_material->find("screen"), 1, _screen);
        float _color[4] = {
                (textColor.rgba >> 24 & 0xFF) * OneOver255,
                (textColor.rgba >> 16 & 0xFF) * OneOver255,
                (textColor.rgba >> 8 & 0xFF) * OneOver255,
                (textColor.rgba & 0xFF) * OneOver255
        };
        glUniform4fv(_material->tintHandle, 1, _color);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, _font->getTexture());
        glUniform1i(_material->textureHandle, 3);

        glEnableVertexAttribArray(_material->vertexHandle);
        glEnableVertexAttribArray(_material->uvHandle);

        glVertexAttribPointer(_material->vertexHandle, 2, GL_FLOAT, false, 0, vertecies.data());
        glVertexAttribPointer(_material->uvHandle, 2, GL_FLOAT, false, 0, uvs.data());

        glDrawArrays(GL_TRIANGLES, 0, vertecies.size() >> 1);

        glDisableVertexAttribArray(_material->vertexHandle);
        glDisableVertexAttribArray(_material->uvHandle);
    }
}

void Seal_AddUIO(int index){
    uioArray[index] = Seal_UIO();
}

void Seal_DeleteUIO(int index){
    uioArray.erase(index);
}

void Seal_SetUIOProp(int index, int propIndex, int value){
    SAFE_SET(index);
    Seal_UIO& uio = uioArray[index];
    switch (propIndex) {
        case I_Texture: uio.texture = value; break;
        case I_Font: uio.font = value; break;
        case I_Material: uio.material = value; break;
        case I_TextMaterial: uio.textMaterial = value; break;
        case I_Color: uio.color = Seal_Color::fromInt(value); break;
        case I_TextColor: uio.textColor = Seal_Color::fromInt(value); break;
        case I_Flags: uio.flags = value;
            Seal_Log("Flags: 0x%X", uio.flags);
            break;
    }
}

void Seal_SetUIOProp(int index, int propIndex, float value){
    SAFE_SET(index);
    Seal_UIO& uio = uioArray[index];
    switch (propIndex) {
        case F_PositionX: uio.transform.x = value; break;
        case F_PositionY: uio.transform.y = value; break;
        case F_Width: uio.transform.w = value; break;
        case F_Height: uio.transform.h = value; break;
        case F_TextSize: uio.textSize = value; break;
        case F_PaddingR: uio.padding.w = value; break;
        case F_PaddingB: uio.padding.h = value; break;
        case F_PaddingL: uio.padding.x = value; break;
        case F_PaddingT: uio.padding.y = value; break;
    }
}

void Seal_SetUIOProp(int index, int propIndex, const char* value){
    SAFE_SET(index);
    Seal_UIO& uio = uioArray[index];
    switch (propIndex) {
        case S_Text:
            uio.text = std::string(value);
            break;
    }
}

void Seal_SetUIOVMode(int index, int vMode){
    SAFE_SET(index);
    Seal_UIO& uio = uioArray[index];
    uio.flags = vMode;
}