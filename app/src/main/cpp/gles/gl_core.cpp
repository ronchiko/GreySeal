
#include <GLES2/gl2.h>
#include <queue>

#include "sealog.h"


#include "gs_android.h"
#include "greyseal/texture.h"
#include "greyseal/gl.h"
#include "greyseal/scene.h"
#include "greyseal/mesh.h"
#include "greyseal/matrix.h"

static std::queue<Seal_Object**> toDestroy;

int Seal_Specs::width = 0;
int Seal_Specs::height = 0;

static Seal_Scene* scene;
Seal_Texture defaultTextureId;

Seal_Object* cube;

Seal_Texture textureId;

float a;

int Seal_GlStart(int width, int height){

    Seal_Specs::width = width; Seal_Specs::height = height;

    glViewport(0, 0, width, height);

    scene = new Seal_Scene();

    cube = new Seal_Object();
    cube->transform.position.z = -15;
    cube->setMaterial(Seal_LoadMaterial("vertex.glsl", "fragment.glsl"));
    cube->setMesh(Seal_LoadMesh("models/cube-nuv.obj"));
    scene->addObject(cube);

    defaultTextureId = Seal_LoadTexture("textures/white");

    textureId = Seal_LoadTexture("textures/seal.jpg");
    cube->setColor(SEAL_BLUE);
    cube->setTexture(textureId);
    return 1;
}

void Seal_PopDestroy(void){
    while(!toDestroy.empty())
    {
        Seal_Object** object = toDestroy.front();
        toDestroy.pop();

        if(*object) {
            // Remove object from scene
            scene->root.erase(std::remove(scene->root.begin(), scene->root.end(), *object), scene->root.end());
            *object = nullptr;  // Set the old pointer to null
            delete *object;
        }
    }
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

void Seal_Render(){
    // Update objects

    // Execute system results

    // Destroy all queue objects before rendering
    Seal_PopDestroy();

    if(scene) {
        scene->drawScene();
        if(cube) {
            cube->transform.rotation.setRotation(a, 1, 1, 1);
            a++;
            if (a > 360)
                Seal_Destroy(&cube);
        }
    }
}

void Seal_Destroy(Seal_Object** object){
    toDestroy.push(object);
}

Seal_Scene& Seal_CurrentScene(void){
    return *scene;
}

void Seal_GlEnd(){
    delete scene;
}