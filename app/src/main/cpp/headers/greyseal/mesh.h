#pragma once

#include <string>

#include <GLES2/gl2.h>

#define SEAL_NO_MESH -1

typedef int Seal_MeshHandle;

/**
 * Represents a 3d mesh
 */
struct Seal_Mesh {
public:
    // explicitly delete copy constructors
    Seal_Mesh(const Seal_Mesh&) = delete;
    Seal_Mesh(Seal_Mesh&&) = delete;

    /**
     * \brief Creates a new mesh
     */
    Seal_Mesh(int, float*, float*, float*);
    ~Seal_Mesh();
    /**
     * \brief Puts the mesh into a shader's data
     *
     * \param vertex The handle of the vertex attribute
     * \param uv The handle for the uv attribute
     * \param normal The handle for the normal attribute
     */
    void intoAttributes(const GLint* vertex, const GLint* uv, const GLint* normal) const;

    inline int size() const { return vertexCount; }
    inline const float* getVertecies() const { return vertecies; }
private:
    int vertexCount;
    float* vertecies;
    float* normals;
    float* uvs;
};

/**
 * \brief loads a mesh from the assets, given a path (.obj file)
 */
int Seal_LoadMesh(const std::string& path);
/**
 * \brief gets a mesh by its loaded index
 */
Seal_Mesh* Seal_GetMesh(int index);
/**
 * \brief gets a mesh by its name, only meshed created with (int loadMesh(const char*))
 */
Seal_Mesh* Seal_GetMeshByPath(const std::string& path);



