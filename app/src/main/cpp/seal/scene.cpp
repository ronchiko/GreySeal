
#include <GLES2/gl2.h>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "greyseal/object.h"
#include "greyseal/scene.h"
#include "greyseal/mesh.h"
#include "greyseal/matrix.h"
#include "greyseal/gl.h"

#include "sealog.h"

const Seal_Vector3 FORWARDS = {0.f, 0.f, 1.f};
const Seal_Vector3 UP = {0.f, 1.f, 0.f};

void Seal_Transform::move(float x, float y, float z) {
    position.x += x;
    position.y += y;
    position.z += z;
}

void Seal_Transform::transformMatrix(float *matrix) {
    Seal_IdentityMatrix(matrix);

    float _rotation[16];
    rotation.toMatrix(_rotation);

    Seal_ScaleMatrix(matrix, scale.x, scale.y, scale.z);
    Seal_MatrixMul(matrix, _rotation, matrix);
    Seal_TranslateMatrix(matrix, position.x, position.y, position.z);
}

void Seal_Scene::push(Seal_Entity *entities, int _count) {
    if(_count <= 0) return;

    start = (Seal_ObjectUnion*)realloc(start, (_count + count) * sizeof(Seal_ObjectUnion));
    for(int i = 0; i < _count; i++){
        start[count + i] = Seal_ObjectUnion(&entities[i]);
        //Seal_Log("Entity: %d %d %f", i, count + i, entities[i].transform.position.x);
    }
    count += _count;
    //Seal_Log("Count: %d, Added: %d", count, _count);
}

void Seal_Scene::cleanse(void){
    // Batch groups of deleted object and override them with the minimum amount of moves
    int newCount = count;
    int removeCount = 0;
    for(int i = count - 1; i >= 1; i--){
        if(start[i]->engineFlags & SEAL_ENGINE_DESTROYED){
            int batchStart = i;
            while(i >= 1 && start[i]->engineFlags & SEAL_ENGINE_DESTROYED){
                i--;
            }
            memmove(start + i + 1, start + batchStart + 1, (newCount - batchStart) * sizeof(Seal_ObjectUnion));
            newCount -= (batchStart - i);
            removeCount += (batchStart - i);
        }else if(start[i]->engineFlags & SEAL_ENGINE_DESTROY){
            start[i]->engineFlags = (start[i]->engineFlags & ~SEAL_ENGINE_DESTROY) | SEAL_ENGINE_DESTROYED;
        }
    }

    start = (Seal_ObjectUnion*)realloc(start, sizeof(Seal_ObjectUnion) * newCount);
    count = newCount;
}

void Seal_Scene::render(void) const {

    glClearColor(0.5f, 0.5f, 1.f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Render world objects
    glEnable(GL_DEPTH_TEST);
    float view[16], identity[16], projection[16];
    Seal_IdentityMatrix(identity);
    viewMatrix(view);
    Seal_MatrixPerspective(projection, 60, Seal_Specs::ratio(), 0.01f, 100.f);
    Seal_MatrixMul(view, projection, view);

    //Seal_Log("Rendering object %d", count);
    for(int i = 1; i < count; i++)
        Seal_RenderObject(&(start[i].object), this, identity, view);

}

void Seal_Scene::viewMatrix(float *matrix) const {
    Seal_MatrixLookAt(matrix, FORWARDS * camera().transform.rotation, camera().transform.position);
}

void Seal_RenderObject(Seal_Entity* object, const Seal_Scene* scene, float* parent, float* view){
    // Check if the process will fail
    if(!object || !scene) return;

    // New objects dont get rendered until the user gets to initialize it with systems
    if(object->engineFlags & SEAL_ENGINE_NEW){
        object->engineFlags &= ~SEAL_ENGINE_NEW;
        return;
    }

    if(object->engineFlags & (SEAL_ENGINE_DONT_DRAW | SEAL_ENGINE_DESTROYED)){
        return;
    }


    // If there is a need for a transform computation, then compute only once
    float transform[16];

    // If there is no mesh or is using a corrupted program, don't Seal_Render
    if(object->mesh > SEAL_NO_MESH && object->material > SEAL_NO_MATERIAL){
        object->transform.transformMatrix(transform);
        Seal_Material* material = Seal_GetMaterial(object->material);
        Seal_Mesh* mesh = Seal_GetMesh(object->mesh);

        // Seal_Log("Mesh: %d, L:%d", object->mesh, mesh->size());
        // Seal_Log("Position: %f, %f, %f", object->transform.position.x, object->transform.position.y, object->transform.position.z);
        // Seal_Log("Rot: %f, %f, %f", object->transform.rotation.x, object->transform.rotation.y, object->transform.rotation.z);
        glUseProgram(material->program);

        // Prepare attributes
        glEnableVertexAttribArray(material->vertexHandle);
        glEnableVertexAttribArray(material->uvHandle);
        glEnableVertexAttribArray(material->normalHandle);
        mesh->intoAttributes(&material->vertexHandle, &material->uvHandle, &material->normalHandle);

        glUniformMatrix4fv(material->modelMatrixHandle, 1, GL_FALSE, transform);
        glUniformMatrix4fv(material->projectionMatrixHandle, 1, GL_FALSE, view);

        float color[4];
        color[0] = (object->color.rgba >> 24 & 0xFF) * OneOver255;
        color[1] = (object->color.rgba >> 16 & 0xFF) * OneOver255;
        color[2] = (object->color.rgba >> 8 & 0xFF) * OneOver255;
        color[3] = (object->color.rgba & 0xFF) * OneOver255;
        glUniform4fv(material->tintHandle, 1, color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, object->texture == SEAL_NO_TEXTURE ? defaultTextureId : object->texture);
        glUniform1i(material->textureHandle, 0);

        // Pass parent matrix
        glDrawArrays(GL_TRIANGLES, 0, mesh->size());

        glDisableVertexAttribArray(material->vertexHandle);
        glDisableVertexAttribArray(material->uvHandle);
        glDisableVertexAttribArray(material->normalHandle);

        glBindTexture(GL_TEXTURE_2D, SEAL_NO_TEXTURE);
    }
}

void Seal_SetCamera(float* transformVector){
    Seal_Scene* scene = Seal_CurrentScene();
    if(scene){
        scene->camera().transform.position = *(Seal_Vector3*)transformVector;
        scene->camera().transform.rotation = Seal_Quaternion(transformVector[3], transformVector[4],
                transformVector[5], transformVector[6]);

//        Seal_Log("Camera: %f %f %f : %f %f %f %f", scene->camera().transform.position.x,
//                 scene->camera().transform.position.y, scene->camera().transform.position.z,
//                 scene->camera().transform.rotation.w, scene->camera().transform.rotation.x,
//                 scene->camera().transform.rotation.y, scene->camera().transform.rotation.z);
    }
}