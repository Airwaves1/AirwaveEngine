#include "mesh.h"

#include "Shader.h"
#include "Utils/utils.h"
#include <qopenglfunctions_4_5_core.h>

#include "Utils/logger.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> texture)
{
    //传入数据
    _vertices = vertices;
    _indices = indices;
    _textures = texture;
    //设置顶点数据，和顶点属性
    setupMesh();

}

//渲染Mesh
void Mesh::draw(Shader shader)
{
    GLuint diffuseNr = 1;	//漫反射贴图编号
    GLuint specularNr = 1;	//镜面贴图编号
    GLuint normalNr = 1;	//法线贴图编号
    GLuint heightNr = 1;	//位移贴图编号

    for (GLuint i = 0; i < this->_textures.size(); ++i)
    {
        OPENGL_FUNCTIONS->glActiveTexture(GL_TEXTURE0 + i); // 激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        std::string number;
        std::string name = _textures[i].type;

        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }
        else if(name == "texture_normal")
        {
            number = std::to_string(normalNr++);
        }
        else if(name == "texture_height")
        {
            number = std::to_string(heightNr++);
        }
        //设置uniform变量
        //shader.setUniformInt((name + number).c_str(), i);
        OPENGL_EXTRA_FUNCTIONS->glUniform1i(OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(shader.getShaderProgramID(), (name + number).c_str()), i);
        OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, _textures[i].id);
    }

    //绘制Mesh
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(m_VAO);
    OPENGL_FUNCTIONS->glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_indices.size()), GL_UNSIGNED_INT, 0);
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);

    //重置纹理单元
    OPENGL_FUNCTIONS->glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    //创建 VAO VBO EBO
    OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &m_VAO);
    OPENGL_FUNCTIONS->glGenBuffers(1, &m_VBO);
    OPENGL_FUNCTIONS->glGenBuffers(1, &m_EBO);

    //绑定VAO
OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(m_VAO);

    //绑定VBO
    OPENGL_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    OPENGL_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

    //绑定EBO
    OPENGL_FUNCTIONS->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    OPENGL_FUNCTIONS->glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), &_indices[0], GL_STATIC_DRAW);

    //设置顶点属性
    //Vertex Positions
    OPENGL_FUNCTIONS->glEnableVertexAttribArray(0);
    OPENGL_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    CHECK_OPENGL_ERROR();

    //Vertex Normals
    OPENGL_FUNCTIONS->glEnableVertexAttribArray(1);
    OPENGL_FUNCTIONS->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    CHECK_OPENGL_ERROR();

    //Vertex Texture Coords
    OPENGL_FUNCTIONS->glEnableVertexAttribArray(2);
    OPENGL_FUNCTIONS->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    CHECK_OPENGL_ERROR();

    //Vertex Tangent
    OPENGL_FUNCTIONS->glEnableVertexAttribArray(3);
    OPENGL_FUNCTIONS->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    CHECK_OPENGL_ERROR();

    //Vertex Bitangent
    OPENGL_FUNCTIONS->glEnableVertexAttribArray(4);
    OPENGL_FUNCTIONS->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    //Vertex Bone IDs
    OPENGL_FUNCTIONS->glEnableVertexAttribArray(5);
    OPENGL_EXTRA_FUNCTIONS->glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    //Vertex Weights
    OPENGL_FUNCTIONS->glEnableVertexAttribArray(6);
    OPENGL_FUNCTIONS->glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

    //解绑VAO
    OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);

    Logger::debug("VAO: %1", m_VAO);
}
