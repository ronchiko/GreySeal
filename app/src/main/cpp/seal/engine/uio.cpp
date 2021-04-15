
#include <unordered_map>
#include <algorithm>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "greyseal/ui.h"
#include "greyseal/gl.h"
#include "greyseal/font.h"

#define MAP_ONE_MINUS_VALUE(value, range) (1 - static_cast<float>(value)/range * 2)
#define MAP_VALUE(value, range) (static_cast<float>(value)/range * 2 - 1)

#define SAFE_SET(index) if(uioArray.find(index) == uioArray.end()) { \
    return;    \
    }

static std::unordered_map<int, Seal_UIO> uioArray;

extern int defaultTextureId;

static float QUAD_UVS[8] =       {  0, 0, 1, 0,  0,  1, 1,  1 };
typedef enum __DrawingFlags {
    Draw = 0x1,
    TextObject = 0x2
} Seal_UioDrawingFlags;

enum UioPropIndecies {
    I_Texture = 0,
    I_Font = 1,
    I_Material = 2,

    F_PositionX = 3,
    F_PositionY = 4,
    F_Width = 5,
    F_Height = 6,

    I_Color = 7,

    S_Text = 8
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

Seal_UIO::Seal_UIO() : flags(), color(SEAL_WHITE), text(), font(), texture(SEAL_NO_TEXTURE),
    material(SEAL_NO_MATERIAL), transform(){
    color = Seal_Color(255, 255, 255, 255);
    transform = { 0, 0, 100, 100 };
    material = Seal_LoadMaterial("vertex-ui.glsl", "fragment-ui.glsl");
    texture = Seal_LoadTexture("textures/white");
}

Seal_UIO::~Seal_UIO() {}

void Seal_UIO::render(void) const {
    if(material > SEAL_NO_MATERIAL && texture > SEAL_NO_TEXTURE) {
        Seal_Material* _material = Seal_GetMaterial(material);
        glUseProgram(_material->program);

        float _vertecies[8] = {
                MAP_VALUE(transform.x, Seal_Specs::width),
                MAP_ONE_MINUS_VALUE(transform.endy(), Seal_Specs::height),
                MAP_VALUE(transform.endx(), Seal_Specs::width),
                MAP_ONE_MINUS_VALUE(transform.endy(), Seal_Specs::height),
                MAP_VALUE(transform.x, Seal_Specs::width),
                MAP_ONE_MINUS_VALUE(transform.y, Seal_Specs::height),
                MAP_VALUE(transform.endx(), Seal_Specs::width),
                MAP_ONE_MINUS_VALUE(transform.y, Seal_Specs::height),
        };

        glEnableVertexAttribArray(_material->vertexHandle);
        glEnableVertexAttribArray(_material->uvHandle);

        glVertexAttribPointer(_material->vertexHandle, 2, GL_FLOAT, false, 0, _vertecies);
        glVertexAttribPointer(_material->uvHandle, 2, GL_FLOAT, false, 0, QUAD_UVS);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(_material->textureHandle, 1);

        float _color[4] = {
                (color.rgba >> 24 & 0xFF) * OneOver255,
                (color.rgba >> 16 & 0xFF) * OneOver255,
                (color.rgba >> 8 & 0xFF) * OneOver255,
                (color.rgba & 0xFF) * OneOver255
        };
        glUniform4fv(_material->tintHandle, 1, _color);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glDisableVertexAttribArray(_material->vertexHandle);
        glDisableVertexAttribArray(_material->uvHandle);

        glBindTexture(GL_TEXTURE_2D, SEAL_NO_TEXTURE);
    }

    /** Draw text */
    if(!text.empty() && font >= SEAL_NO_FONT){
        int arrayLength = text.length() * 6;
        float* vertecies = new float[arrayLength], *uvs = new float[arrayLength];

        float _start = MAP_VALUE(transform.x, Seal_Specs::width);
        float _x = _start, _y = MAP_ONE_MINUS_VALUE(transform.y, Seal_Specs::height);

        Seal_Font* _font = Seal_GetFont(font);
        for(auto& c : text){

        }

        delete[] vertecies;
        delete[] uvs;
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
        case I_Color: uio.color = Seal_Color::fromInt(value); break;
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