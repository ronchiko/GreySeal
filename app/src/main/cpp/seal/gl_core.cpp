
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <vector>
#include "sealog.h"

#include "greyseal/initialize.h"
#include "gs_android.h"
#include "greyseal/texture.h"
#include "greyseal/gl.h"
#include "greyseal/scene.h"
#include "greyseal/mesh.h"
#include "greyseal/matrix.h"
#include "greyseal/calls.h"
#include "greyseal/preset.h"
#include "greyseal/ui.h"

int Seal_Specs::width = 0;
int Seal_Specs::height = 0;

static Seal_Scene* scene;
static uint16_t objectUid;

Seal_Texture defaultTextureId;

int Seal_GlStart(){
    Seal_Log("Preparing rnd");
    time_t t;
    srand((unsigned)time(&t));

    Seal_Log("Starting internal GL scene");

    scene = new Seal_Scene();
    scene->camera().transform.rotation = Seal_Quaternion::euler(351.f, 0, 0);
    Seal_Log("Loading textures...");
    defaultTextureId = Seal_LoadWhiteTexture();
    // Initialize player
    return SEAL_SUCCESS;
}


static GLuint Seal_CompileShader(Seal_C_String path, GLenum shaderType){

    char* source;
    int length = Seal_LoadTxt(path, &source);
    if(length == -1) return 0;

    // Compile the shader
    Seal_Log("Loading shader %s", path);
    GLuint shader = glCreateShader(shaderType);
    if(shader){
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        /* Check compile status */
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if(!compiled){
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

            if(logLength){
                char* info = new char[logLength];
                glGetShaderInfoLog(shader, logLength, nullptr, info);

                Seal_LogError("Failed to compile shader %s:\n%s\n", path, info);

                delete[] info;
            }

            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

GLuint Seal_CompileProgram(const Seal_String& vertexPath, const Seal_String& fragmentPath){
    GLuint vertex = Seal_CompileShader(vertexPath.c_str(), GL_VERTEX_SHADER);
    if(!vertex) return 0;
    GLuint fragment = Seal_CompileShader(fragmentPath.c_str(), GL_FRAGMENT_SHADER);
    if(!fragment) return 0;

    GLuint program = glCreateProgram();
    if(program){
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        glLinkProgram(program);

        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if(linkStatus != GL_TRUE){
            GLint length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

            if(length){
                char* info = new char[length];
                glGetProgramInfoLog(program, length, nullptr, info);

                Seal_Log("Failed to create program (vertex: %s, fragment: %s):\n%s\n", vertexPath.c_str(), fragmentPath.c_str(), info);

                delete[] info;
            }

            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void Seal_Render(Seal_Byte* updatedBytes, Seal_Byte* calls, size_t callArrayLength){
    // First we update the objects info
    memcpy(scene->start, updatedBytes, scene->bytes());
    Seal_ExecuteEngineCalls(calls, callArrayLength);

    if(scene) {
        scene->cleanse();
        scene->render();
        Seal_DrawUI();
        Seal_LateInitialize();
    }
}

void Seal_Destroy(Seal_Entity** object){
    (*object)->engineFlags |= SEAL_ENGINE_DESTROY;
    *object = nullptr;
}

void Seal_Instantiate(Seal_Entity** ptr){
    *ptr = &Seal_QueueLateEntity();
    (*ptr)->uid = objectUid++;
}

Seal_Scene* Seal_CurrentScene(void){
    return scene;
}

Seal_Entity* Seal_Find(Seal_Short id){
    for(int i = 1; i < scene->count; i++)
        if(scene->start[i].object.uid == id) return &scene->start[i].object;
    return nullptr;
}

Seal_Entity* Seal_Clone(Seal_Entity* e){
    Seal_Entity* ptr;

    Seal_Instantiate(&ptr);
    uint16_t uid = ptr->uid;
    Seal_EngineFlags flags = ptr->engineFlags;

    memcpy(ptr, e, sizeof(Seal_Entity));
    ptr->uid = uid; ptr->engineFlags = flags;

    return ptr;
}

void Seal_GlEnd(){
    delete scene;
}