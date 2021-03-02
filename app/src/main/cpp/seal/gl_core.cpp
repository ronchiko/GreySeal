
#include <GLES2/gl2.h>

#include "sealog.h"

#include "gs_android.h"
#include "greyseal/texture.h"
#include "greyseal/gl.h"
#include "greyseal/scene.h"
#include "greyseal/mesh.h"
#include "greyseal/matrix.h"
#include "greyseal/calls.h"
#include "greyseal/preset.h"

int Seal_Specs::width = 0;
int Seal_Specs::height = 0;

static Seal_Scene* scene;
static uint16_t objectUid;

Seal_Texture defaultTextureId;
Seal_Entity* cube;
Seal_Texture textureId;

int Seal_GlStart(int width, int height){
    Seal_Log("Preparing rnd");
    time_t t;
    srand((unsigned)time(&t));

    Seal_Log("Starting internal GL scene");
    Seal_Specs::width = width; Seal_Specs::height = height;
    glViewport(0, 0, width, height);
    scene = new Seal_Scene();
    scene->camera.generateMatrix();
    Seal_Log("Loading textures...");
    defaultTextureId = Seal_LoadTexture("textures/white");
    textureId = Seal_LoadTexture("textures/spaceship-uvl.png");


    Seal_Log("Loading presets");
    Seal_Preset preset = Seal_LoadCachePreset("presets/green-cube.ntt");
    Seal_Log("Instatiating objects");
    // Initialize player
    Seal_Entity* playerNtt = nullptr;
    Seal_Instantiate(&playerNtt);
    playerNtt->transform.position = {0, 0, -20.f};

    if(playerNtt) {
//        playerNtt->flags = 0x3; // TODO: Clean temporary flags
//        playerNtt->setMaterial(Seal_LoadMaterial("vertex.glsl", "fragment.glsl"));
//        playerNtt->setMesh(Seal_LoadMesh("models/spaceship.obj"));
//        playerNtt->transform.rotation = Seal_Quaternion::euler(-90, -90, 0);
//
//        playerNtt->setTexture(textureId);
        Seal_OverwriteObject(playerNtt, preset, false);
    }
    return 1;
}

void Seal_PopDestroy(void){
    // Shift the memory according to the shifts
    size_t newObjects = scene->objects;
    for(size_t i = 0; i < scene->objects; i++){
        if(scene->root[i].object.engineFlags & SEAL_ENGINE_DESTROY){
            if(i < scene->objects - 1) {
                // If not last item move the array backwards
                void* nextAddress = &scene->root[i + 1];
                std::memmove(&scene->root[i], nextAddress, (scene->objects - i) * sizeof(Seal_Entity));
            }
            newObjects--;
        }
    }
    scene->root = (Seal_ObjectUnion*)realloc(scene->root, newObjects * sizeof(Seal_Entity));
    scene->objects = newObjects;
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
    memcpy(scene->root, updatedBytes, scene->bytes());
    Seal_ExecuteEngineCalls(calls, callArrayLength);
    // Destroy all queue objects before rendering
    Seal_PopDestroy();

    if(scene) {
        scene->drawScene();
    }
}

void Seal_Destroy(Seal_Entity** object){
    (*object)->engineFlags |= SEAL_ENGINE_DESTROY;
    *object = nullptr;
}

void Seal_Instantiate(Seal_Entity** ptr){
    scene->root = (Seal_ObjectUnion*)realloc(scene->root, (scene->objects + 1) * sizeof(Seal_ObjectUnion));
    scene->root[scene->objects++] = Seal_ObjectUnion();
    *ptr = (Seal_Entity*)&scene->root[scene->objects - 1];
    (*ptr)->engineFlags |= SEAL_FLAG_NEW;
    (*ptr)->uid = objectUid++;
}

Seal_Scene* Seal_CurrentScene(void){
    return scene;
}

Seal_Entity* Seal_Find(Seal_Short id){
    for(int i = 0; i < scene->objects; i++)
        if(scene->root[i].object.uid == id) return &scene->root[i].object;
    return nullptr;
}

void Seal_GlEnd(){
    delete scene;
}