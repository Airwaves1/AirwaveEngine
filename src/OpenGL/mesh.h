#pragma once

#include <QVector3D>
#include <qvector2d.h>
#include <QMatrix4x4>
#include <qmath.h>
#include <qopenglfunctions.h>
#include <string>
#include <vector>

#include <assimp/types.h>

#define MAX_BONE_INFLUENCE 4

class Shader;

//网格类
struct Vertex
{
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;

    QVector3D Tangent; // 切线
    QVector3D Bitangent; // 双切线

    //骨骼ID
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //骨骼权重
    float m_Weights[MAX_BONE_INFLUENCE];

};

struct Texture
{
    GLuint id;
    std::string type;
    std::string path;  // 储存纹理的文件位置
};

class Mesh
{
public:
    //网格数据
    std::vector<Vertex> _vertices;
    std::vector<GLuint> _indices;
    std::vector<Texture> _textures;
    GLuint m_VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> texture);
    void draw(Shader shader);

private:
    //渲染数据
    GLuint m_VBO, m_EBO;
    //初始化网格数据
    void setupMesh();
};

