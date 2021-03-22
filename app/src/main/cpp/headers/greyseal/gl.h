#pragma once

#include <GLES2/gl2.h>

#include "scene.h"

namespace Seal_Specs {
    extern int width, height;

    inline float ratio(void) { return (float)width / height; }
};

extern Seal_Texture defaultTextureId;

/**
 * \brief start OpenGL ES 2.0
 */
int Seal_GlStart(int width, int height);
/**
 * \brief Ends the engine, and also frees all the allocated memory
 */
void Seal_GlEnd();
/**
 * \brief compiles 2 shaders and makes program
 *
 * \return returns the program id, or 0 if failed
 */
GLuint Seal_CompileProgram(const Seal_String& vertex, const Seal_String& fragment);

/**
 * \brief renders the frame
 */
void Seal_Render(Seal_Byte* updatedData, Seal_Byte* commands, size_t);
Seal_Scene* Seal_CurrentScene(void);