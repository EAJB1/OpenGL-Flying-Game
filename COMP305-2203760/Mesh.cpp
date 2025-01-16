#include "Mesh.h"

// Mesh constructor
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::Draw(Shader& shader) const
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    bool hasTexture = false;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        // Active proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);

        // Retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
        {
            hasTexture = true;
            number = std::to_string(diffuseNr++);
        }
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        // Set the sampler to the correct texture unit, using the location of a uniform variable
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    shader.setBool("hasTexture", hasTexture);

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

/// <summary>
/// Initializes all the buffer objects/arrays.
/// (set the vertex buffers and its attribute pointers)
/// </summary>
void Mesh::setupMesh()
{
// Initialize buffers/arrays
glGenVertexArrays(1, &VAO); // Vertex Array Object
glGenBuffers(1, &VBO); // Vertex Buffer Object
glGenBuffers(1, &EBO); // Element Buffer Object

// Tell OpenGL to use the VAO
glBindVertexArray(VAO);

// Load data into vertex buffers
glBindBuffer(GL_ARRAY_BUFFER, VBO);

// Creates and initializes a buffer object's data store, using the struct
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

// Vertex Positions
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

// Vertex normals
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

// Vertex texture coords
glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

// Vertex tangent
glEnableVertexAttribArray(3);
glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

// Vertex bitangent
glEnableVertexAttribArray(4);
glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

// IDs
glEnableVertexAttribArray(5);
glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

// Weights
glEnableVertexAttribArray(6);
glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
glBindVertexArray(0);
}
