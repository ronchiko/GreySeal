
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <strstream>

#include "greyseal/vector3.h"
#include "gs_android.h"
#include "greyseal/mesh.h"

#include "sealog.h"

static std::unordered_map<int, Seal_Mesh*> meshes;
static std::unordered_map<std::string, int> loadedMeshes;
static int __meshIndex = 0;

Seal_Mesh::Seal_Mesh(int c, float *v, float *u, float *n) : vertexCount(c), vertecies(v), uvs(u), normals(n) {}
Seal_Mesh::~Seal_Mesh() {
    delete [] vertecies;
    delete [] uvs;
    delete [] normals;
}

void Seal_Mesh::intoAttributes(const GLint *vertex, const GLint *uv, const GLint *normal) const {
    if(vertex) glVertexAttribPointer(*vertex, 3, GL_FLOAT, false, 0, vertecies);
    if(uv) glVertexAttribPointer(*uv, 2, GL_FLOAT, false, 0, uvs);
    if(normal) glVertexAttribPointer(*normal, 3, GL_FLOAT, false, 0, normals);
}

Seal_Mesh* Seal_GetMesh(int index){
    if(meshes.find(index) == meshes.end() || index == -1)
        return nullptr;
    return meshes[index];
}

Seal_Mesh* Seal_GetMeshByPath(const std::string& path){
    if(loadedMeshes.find(path) == loadedMeshes.end()){
        return nullptr;
    }
    return Seal_GetMesh(loadedMeshes[path]);
}

int Seal_LoadMesh(const std::string& path){
    if(loadedMeshes.find(path) != loadedMeshes.end()) return loadedMeshes[path];

    Seal_Log("Loading mesh %s", path.c_str());
    char* raw;
    int length = Seal_LoadTxt(path.c_str(), &raw);

    if(length >= 0){
        std::vector<Seal_Vector3> vIndex, nIndex, uIndex;
        nIndex.emplace_back(); uIndex.emplace_back();
        std::vector<float> vertecies, uvs, normals;

        std::stringstream input;
        input.str(raw);

        for(std::string line; std::getline(input, line);) {
            std::strstream stream; stream << line;
            char h;
            stream >> h;
            switch(h){
                case 'v': {
                    Seal_Vector3 v;
                    stream.read(&h, 1);     //Using read this time to catch the space character
                    switch (h) {
                        case ' ':
                            stream >> v.x >> v.y >> v.z;
                            vIndex.push_back(v);
                            break;
                        case 'n':
                            stream >> v.x >> v.y >> v.z;
                            nIndex.push_back(v);
                            break;
                        case 't':
                            stream >> v.x >> v.y;
                            uIndex.push_back(v);
                            break;
                        default:
                            break;
                    }
                }break;
                case 'f': {
                    for(int _ = 0; _ < 3; _++){
                        int indecies[3] = { -1, 0, 0};

                        for(int j = 0; j < 3; j++){
                            if(stream.peek() == '/')  {
                                stream >> h;
                                continue;
                            }
                            stream >> indecies[j];

                            if(j != 2) stream >> h;
                        }

                        int v = indecies[0], u = indecies[1], n = indecies[2];

                        vertecies.push_back(vIndex[v - 1].x);
                        vertecies.push_back(vIndex[v - 1].y);
                        vertecies.push_back(vIndex[v - 1].z);

                        uvs.push_back(uIndex[u].x); uvs.push_back(uIndex[u].y);

                        normals.push_back(nIndex[n].x);
                        normals.push_back(nIndex[n].y);
                        normals.push_back(nIndex[n].z);
                    }
                }break;
                default:
                    break;
            }
        }

        int index = __meshIndex++;

        int size = (int)vertecies.size() / 3;
        auto *vertexArray = new float[size * 3], *uvArray = new float[size * 2], *normalArray = new float[size *  3];

        std::copy(vertecies.begin(), vertecies.end(), vertexArray);
        std::copy(uvs.begin(), uvs.end(), uvArray);
        std::copy(normals.begin(), normals.end(), normalArray);
        meshes[index] = new Seal_Mesh(size, vertexArray, uvArray, normalArray);
        loadedMeshes[path] = index;

        delete [] raw;
        Seal_Log("Loaded %d %d %d", vertecies.size() / 3, uvs.size() / 2, normals.size() / 3);
        Seal_Log("Successfully loaded mesh %s", path.c_str());
        return index;
    }

    Seal_LogError("Failed to load mesh %s.", path.c_str());
    return -1;
}