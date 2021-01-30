
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

    Seal_MatrixMul(matrix, _rotation, matrix);
    Seal_ScaleMatrix(matrix, scale.x, scale.y, scale.z);
    Seal_TranslateMatrix(matrix, position.x, position.y, position.z);
}

Seal_Object::~Seal_Object() {
    for(Seal_Object*& child : children) {
        delete child;
    }
}

void Seal_Object::setMesh(int _mesh) {
    mesh = _mesh;
}
void Seal_Object::setMaterial(int m) {
    material = m;
}
void Seal_Camera::generateMatrix() {
    Seal_MatrixPerspective(matrix, FOV, (float) Seal_Specs::width / (float) Seal_Specs::height, Z_NEAR, Z_FAR);
}

void Seal_RenderObject(Seal_Object* object, Seal_Scene* scene, float* parent){
    // Check if the process will fail
    if(!object || !scene) return;

    // If there is a need for a transform computation, then compute only once
    float transform[16];
    if((object->mesh >= SEAL_NO_MESH && object->material >= SEAL_NO_MATERIAL) || !object->children.empty()) {
        object->transform.transformMatrix(transform);
    }

    // If there is no mesh or is using a corrupted program, don't Seal_Render
    if(object->mesh > SEAL_NO_MESH && object->material > SEAL_NO_MATERIAL){

        Seal_Material* material = Seal_GetMaterial(object->material);
        Seal_Mesh* mesh = Seal_GetMesh(object->mesh);

        glUseProgram(material->program);

        // Prepare attributes
        glEnableVertexAttribArray(material->vertexHandle);
        glEnableVertexAttribArray(material->uvHandle);
        glEnableVertexAttribArray(material->normalHandle);
        mesh->intoAttributes(&material->vertexHandle, &material->uvHandle, &material->normalHandle);

        glUniformMatrix4fv(material->modelMatrixHandle, 1, GL_FALSE, transform);
        glUniformMatrix4fv(material->projectionMatrixHandle, 1, GL_FALSE, scene->camera.getMatrix());

        glUniform4fv(material->tintHandle, 1, object->color.rgba);

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
/*
    // Render children if possible
    if(!object->children.empty()){
        float newParent[16];
        Seal_MatrixMul(newParent, parent, transform);
        for(auto& child : object->children)
            Seal_RenderObject(child, scene, newParent);
    }*/
}

Seal_Scene::~Seal_Scene() {
    delete &root;
    delete &camera;
}

void Seal_Scene::drawScene() {
    glClearColor(0, 255, 255, 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the over world
    glEnable(GL_DEPTH_TEST);

    float parent[16];
    Seal_IdentityMatrix(parent);
    for(Seal_Object*& object : root)
        Seal_RenderObject(object, this, parent);

    glDisable(GL_DEPTH_TEST);
}

void Seal_Scene::addObject(Seal_Object *object) {
    root.push_back(object);
}

int Seal_Reparent(Seal_Object* child, Seal_Object* parent){
    // Parent cannot be the child itself, or we are trying to move null
    if(child == parent || !child) return -1;

    // Erase the child from the old parent
    std::vector<Seal_Object*>& oldFamily = child->parent->children;
    oldFamily.erase(std::remove(oldFamily.begin(), oldFamily.end(), child), oldFamily.end());

    //If the child is moved to null, then move it to the scene root
    /*if(!parent)
        parent = &Seal_CurrentScene().root;*/

    // Set the child parent to the new parent, and the child to the parent children vector
    child->parent = parent;
    parent->children.push_back(child);
    return 0;
}

int Seal_Deparent(Seal_Object* child){
    if(child){
        // Erase the child from the old parent
        std::vector<Seal_Object*>& oldFamily = child->parent->children;
        oldFamily.erase(std::remove(oldFamily.begin(), oldFamily.end(), child), oldFamily.end());

        child->parent = nullptr;
        return 0;
    }
    return -1;
}