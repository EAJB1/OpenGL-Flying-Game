#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.h"

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    // Bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // Weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    // Type could be diffuse or specular.
    string type;
    string path;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

    // Mesh constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    void Draw(Shader& shader) const;

private:
    // Render data 
    unsigned int VBO, EBO;

    /// <summary>
    /// Initializes all the buffer objects/arrays.
    /// (set the vertex buffers and its attribute pointers)
    /// </summary>
    void setupMesh();
};
