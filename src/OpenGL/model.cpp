#include "model.h"

#include <qvector3d.h>
#include <qvector2d.h>
#include <QFile>
#include <qopenglfunctions_4_5_core.h>

#include "Shader.h"

#include "stb_image.h"
#include  "MyTexture.h"

#include "Utils/utils.h"
#include "Utils/logger.h"


Model::Model(const std::string &path, bool gamma) : gammaCorrection(gamma)
{
    Logger::debug("Model::Model() path: %1", path.c_str());
    loadModel(path);
}

Model::~Model()
{
}

void Model::draw(Shader &shader)
{
    for(unsigned int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].draw(shader);
    }
}

void Model::loadModel(const std::string& path)
{

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

    //如果不是完整的场景或者根节点为空
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Logger::error("ERROR::ASSIMP::", importer.GetErrorString());
        return;
    }
    //获取模型文件所在目录
    m_directory = path.substr(0, path.find_last_of('/'));
    Logger::debug("Model::loadModel() m_directory: %1", m_directory.c_str());

    //递归处理根节点
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    //添加当前节点中的所有Mesh
    for(unsigned int i = 0; i< node->mNumMeshes; i++)
    {
        //获取当前节点的第i个Mesh
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    //递归处理该节点的子孙节点
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    //数据
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        QVector3D vector;	//声明一个QVector3D对象，用于处理顶点坐标，法线和纹理坐标
        //处理顶点坐标，法线和纹理坐标
        //Position
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.Position = vector;			//将QVector3D对象的值赋给Vertex结构体的Position成员

        //Normal
        if (mesh->HasNormals())	//如果存在法线
        {
            vector.setX(mesh->mNormals[i].x);
            vector.setY(mesh->mNormals[i].y);
            vector.setZ(mesh->mNormals[i].z);
            vertex.Normal = vector;
        }


        //Texture Coordinates
        if (mesh->mTextureCoords[0])	//如果存在纹理坐标
        {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vec;

            //tangent
            vector.setX(mesh->mTangents[i].x);
            vector.setY(mesh->mTangents[i].y);
            vector.setZ(mesh->mTangents[i].z);
            vertex.Tangent = vector;

            //Bitangent
            vector.setX(mesh->mBitangents[i].x);
            vector.setY(mesh->mBitangents[i].y);
            vector.setZ(mesh->mBitangents[i].z);
            vertex.Bitangent = vector;
        }
        else
        {
            vertex.TexCoords = QVector2D(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    //处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    //处理材质
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // 我们假设在着色器中有一个采样器名称的约定。
    // 每个漫反射纹理应该命名为“texture diffuseN”，其中N是一个从1到MAX SAMPLER number的序列号。
    // 下面的列表总结了同样适用于其他纹理:
    // diffuse: texture_diffuseN
    // specular: texture_specular
    // Nnormal: texture_normalN

    //1.漫反射贴图
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    //2.镜面贴图
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    //3.法线贴图
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    //4.位移贴图
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    //返回一个完整的网格
    return Mesh(vertices, indices, textures);
}

//检查给定类型的所有材质纹理，并加载纹理，如果它们没有加载，则需要的信息作为纹理结构返回。
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        //检查纹理是否已经被加载，如果没有，加载它
        bool skip = false;
        for (unsigned int j = 0; j < m_textures_loaded.size(); j++)
        {
            if (std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_textures_loaded.push_back(texture);
        }
    }
    return textures;
}

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    GLuint textureID;
    OPENGL_FUNCTIONS->glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format = 0;
        if (nrComponents == 1)
        {
            format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }

        OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, textureID);
        OPENGL_FUNCTIONS->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        OPENGL_FUNCTIONS->glGenerateMipmap(GL_TEXTURE_2D);

        OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        Logger::error("Texture failed to load at path: %1", path);
        stbi_image_free(data);
    }

    return textureID;
}



