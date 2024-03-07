#pragma once

#include <vector>
#include <QVector3D>
#include <qvector2d.h>
#include <QMatrix4x4>
#include <qmath.h>
#include <qopenglfunctions.h>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

class Shader;

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
class Model
{
public:
    //模型数据
    std::vector<Texture> m_textures_loaded;
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    bool gammaCorrection = false;   //是否进行gamma矫正

    Model(const std::string& path, bool gamma = false);
    ~Model();

    void draw(Shader &shader);

private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:


};

