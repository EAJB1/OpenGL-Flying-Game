#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "stb_image.h"

#include "Mesh.h"
#include "Shader.h"

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    // Textures won't load more than once, all textures are loaded globally.
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    // Model constructor. Filepath of 3D model must be supported by Assimp, e.g. FBX, OBJ, BLEND, etc.
    Model(string const& path);

    /// <summary>
    /// Cycle through each mesh in the model and draw.
    /// </summary>
    void Draw(Shader& shader) const;

private:
    /// <summary>
    /// Load model (called in constructor) into scene object.
    /// </summary>
    void loadModel(string const& path);

    /// <summary>
    /// Process all the scene's nodes using a recursive function.
    /// </summary>
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    
    /// <summary>
    /// Load material textures if they have not been loaded.
    /// </summary>
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
