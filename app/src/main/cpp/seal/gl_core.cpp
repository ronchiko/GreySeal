
#include <GLES2/gl2.h>

#include "sealog.h"

#include "gs_android.h"
#include "greyseal/texture.h"
#include "greyseal/gl.h"
#include "greyseal/scene.h"
#include "greyseal/mesh.h"
#include "greyseal/matrix.h"

int Seal_Specs::width = 0;
int Seal_Specs::height = 0;

static Seal_Scene* scene;
static uint16_t objectUid;

Seal_Texture defaultTextureId;
Seal_Object* cube;
Seal_Texture textureId;

int Seal_GlStart(int width, int height){
    Seal_Specs::width = width; Seal_Specs::height = height;
    glViewport(0, 0, width, height);
    scene = new Seal_Scene();

    defaultTextureId = Seal_LoadTexture("textures/white");
    textureId = Seal_LoadTexture("textures/seal.jpg");

    Seal_Instantiate(&cube, {0.f, 0.f, -15.f});

    cube->setMaterial(Seal_LoadMaterial("vertex.glsl", "fragment.glsl"));
    cube->setMesh(Seal_LoadMesh("models/cube-nuv.obj"));

    time_t t;
    srand((unsigned)time(&t));
    cube->setColor(SEAL_BLUE);
    cube->setTexture(textureId);

    // cube->flags = 0x1;
    for(int i = 0; i < 100; i++){
        Seal_Object* object;
        Seal_Instantiate(&object, Seal_Vector3(rand() % 4 - 2, rand() % 4 - 2, rand() % 3 * -3 - 5));
        object->setMesh(Seal_LoadMesh("models/cube-nuv.obj"));
        object->transform.scale = Seal_Vector3(.3f, .3f, .3f);
        object->setMaterial(Seal_LoadMaterial("vertex.glsl", "fragment.glsl"));
        object->setColor(SEAL_GREEN);
        object->flags = 0x1;
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
                std::memmove(&scene->root[i], nextAddress, (scene->objects - i) * sizeof(Seal_Object));
            }
            newObjects--;
        }
    }
    scene->root = (Seal_ObjectUnion*)realloc(scene->root, newObjects * sizeof(Seal_Object));
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

void Seal_Render(Seal_Byte* updatedBytes, Seal_Byte*){

    memcpy(scene->root, updatedBytes, scene->bytes());

    // Destroy all queue objects before rendering
    Seal_PopDestroy();

    if(scene) {
        scene->drawScene();
    }
}

void Seal_Destroy(Seal_Object** object){
    (*object)->engineFlags |= SEAL_ENGINE_DESTROY;
    *object = nullptr;
}

void Seal_Instantiate(Seal_Object** ptr, const Seal_Vector3& position){
    scene->root = (Seal_ObjectUnion*)realloc(scene->root, (scene->objects + 1) * sizeof(Seal_ObjectUnion));
    scene->root[scene->objects++] = Seal_ObjectUnion();
    *ptr = (Seal_Object*)&scene->root[scene->objects - 1];
    (*ptr)->transform.position = position;
}

Seal_Scene* Seal_CurrentScene(void){
    return scene;
}

void Seal_GlEnd(){
    delete scene;
}