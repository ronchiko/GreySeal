
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

void Seal_Camera::generateMatrix() {
    float view[16], perspective[16];
    Seal_Vector3 forwards(0, 0, 1);
    forwards = forwards * transform.rotation;
    Seal_MatrixLookAt(view, forwards, transform.position);
    Seal_MatrixPerspective(perspective, FOV, (float)Seal_Specs::width / (float) Seal_Specs::height, Z_NEAR, Z_FAR);
    Seal_MatrixMul(matrix, perspective, view);
}

void Seal_RenderObject(Seal_Entity* object, Seal_Scene* scene, float* parent){
    // Check if the process will fail
    if(!object || !scene) return;
    // If there is a need for a transform computation, then compute only once
    float transform[16];
    if((object->mesh >= SEAL_NO_MESH && object->material >= SEAL_NO_MATERIAL)) {
        object->transform.transformMatrix(transform);
    }


    // If there is no mesh or is using a corrupted program, don't Seal_Render
    if(object->mesh > SEAL_NO_MESH && object->material > SEAL_NO_MATERIAL){
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
        glUniformMatrix4fv(material->projectionMatrixHandle, 1, GL_FALSE, scene->camera.getMatrix());

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

    // If the object is rendered, then its no longer new
    object->engineFlags &= ~SEAL_FLAG_NEW;
}

Seal_Scene::~Seal_Scene() {
    std::free(root);
    delete &camera;
}

void Seal_Scene::drawScene() {
    glClearColor(15/255.f, 15/255.f, 30/255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the over world
    glEnable(GL_DEPTH_TEST);

    float parent[16];
    Seal_IdentityMatrix(parent);
    for(size_t i = 0; i < objects; i++)
        if((root[i].object.engineFlags & SEAL_ENGINE_DESTROY) == 0)
            Seal_RenderObject(&root[i].object, this, parent);

    glDisable(GL_DEPTH_TEST);
}

void Seal_Scene::addObject(Seal_Entity **object) {
    root = (Seal_ObjectUnion*)realloc(root, (objects + 1) * sizeof(Seal_ObjectUnion));
    root[objects++] = Seal_ObjectUnion(*object);
    delete *object;
    *object = (Seal_Entity*)&root[objects - 1];
}