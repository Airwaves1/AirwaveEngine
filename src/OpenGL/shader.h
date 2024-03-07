#ifndef SHADER_H
#define SHADER_H

#include <qmatrix4x4.h>
#include <qvector3d.h>

#include "Utils/utils.h"

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void begin();	//开始使用当前Shader
    void end();		//停止使用当前Shader

private:
    void checkShaderError(GLuint TargetShader, std::string type);


public:
    /* 设置uniform变量 */
    inline GLuint getLocation(const std::string& name) const;
    inline void setUniformInt(const std::string& name, int value);
    inline void setUniformFloat(const std::string& name, float value);
    inline void setUniformVec2(const std::string& name, const QVector2D& value);
    inline void setUniformVec2(const std::string& name, float x, float y);
    inline void setUniformVec3(const std::string& name, const QVector3D& value);
    inline void setUniformVec3(const std::string& name, float x, float y, float z);
    inline void setUniformMat4(const std::string& name, const QMatrix4x4& value);

    GLuint getShaderProgramID() const { return m_shaderProgram; }


private:
    GLuint m_shaderProgram;
    std::string m_vertexCode;
    std::string m_fragmentCode;
    const char* m_vertexPath;
    const char* m_fragmentPath;
};

inline GLuint Shader::getLocation(const std::string& name) const
{
    //1.通过名称拿到Uniform变量的位置Location
    return OPENGL_EXTRA_FUNCTIONS->glGetUniformLocation(m_shaderProgram, name.c_str());
}

inline void Shader::setUniformInt(const std::string& name, int value)
{
    OPENGL_EXTRA_FUNCTIONS->glUniform1i(getLocation(name), value);
}

inline void Shader::setUniformFloat(const std::string& name, float value)
{
    OPENGL_EXTRA_FUNCTIONS->glUniform1f(getLocation(name), value);
}

inline void Shader::setUniformVec2(const std::string& name, const QVector2D& value)
{
    OPENGL_EXTRA_FUNCTIONS->glUniform2f(getLocation(name), value.x(), value.y());
}

inline void Shader::setUniformVec2(const std::string& name, float x, float y)
{
    OPENGL_EXTRA_FUNCTIONS->glUniform2f(getLocation(name), x, y);
}

inline void Shader::setUniformVec3(const std::string& name, const QVector3D& value)
{
    OPENGL_EXTRA_FUNCTIONS->glUniform3f(getLocation(name), value.x(), value.y(), value.z());
}

inline void Shader::setUniformVec3(const std::string& name, float x, float y, float z)
{
    OPENGL_EXTRA_FUNCTIONS->glUniform3f(getLocation(name), x, y, z);
}

inline void Shader::setUniformMat4(const std::string& name, const QMatrix4x4& value)
{
    OPENGL_EXTRA_FUNCTIONS->glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, value.constData());
}





#endif // SHADER_H
