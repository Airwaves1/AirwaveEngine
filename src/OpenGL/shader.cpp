#include "shader.h"

#include <sstream>
#include <fstream>

#include "Utils/logger.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // 准备Shader代码，从文件中读取Shader代码
    // 从文件中读取Shader代码
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    //确保文件流对象可以抛出异常,文件读取失败和文件流异常，都会抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        //判断文件是否打开成功
        if (!vShaderFile.is_open() || !fShaderFile.is_open())
        {
            Logger::error("Shader::Shader: open file failed");
        }

        //将文件流输入到为字符串流中
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //关闭文件
        vShaderFile.close();
        fShaderFile.close();

        //将字符串流转换为字符串
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        Logger::error("Shader::Shader: read file failed");
    }

    //将字符串转换为C风格字符串
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();


    //***********************************************************************************
    //准备Shader程序

    //1.创建Shader程序
    GLuint vertexShader, fragmentShader;
    vertexShader = OPENGL_EXTRA_FUNCTIONS->glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = OPENGL_EXTRA_FUNCTIONS->glCreateShader(GL_FRAGMENT_SHADER);

    //2.为Shader程序绑定Shader代码
    OPENGL_EXTRA_FUNCTIONS->glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    OPENGL_EXTRA_FUNCTIONS->glShaderSource(fragmentShader, 1, &fragmentShaderSource,NULL);

    //3.编译Shader代码,并检查Shader编译是否成
    OPENGL_EXTRA_FUNCTIONS->glCompileShader(vertexShader);
    checkShaderError(vertexShader, "COMPILE");
    OPENGL_EXTRA_FUNCTIONS->glCompileShader(fragmentShader);
    checkShaderError(fragmentShader, "COMPILE");

    //4.链接Shader程序
    m_shaderProgram = OPENGL_EXTRA_FUNCTIONS->glCreateProgram();
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(m_shaderProgram, vertexShader);
    OPENGL_EXTRA_FUNCTIONS->glAttachShader(m_shaderProgram, fragmentShader);
    OPENGL_EXTRA_FUNCTIONS->glLinkProgram(m_shaderProgram);
    checkShaderError(m_shaderProgram, "LINK");

    //5.删除Shader
    OPENGL_EXTRA_FUNCTIONS->glDeleteShader(vertexShader);
    OPENGL_EXTRA_FUNCTIONS->glDeleteShader(fragmentShader);
    //***********************************************************************************

    Logger::debug(" Shader::Shader: Shader program created successfully");
}

Shader::~Shader()
{
    OPENGL_EXTRA_FUNCTIONS->glDeleteProgram(m_shaderProgram);
}

void Shader::begin()
{
    OPENGL_EXTRA_FUNCTIONS->glUseProgram(m_shaderProgram);
}

void Shader::end()
{
    OPENGL_EXTRA_FUNCTIONS->glUseProgram(0);
}

void Shader::checkShaderError(GLuint TargetShader, std::string type)
{
    int success;
    char infoLog[512];
    if (type == "COMPILE")
    {
        OPENGL_EXTRA_FUNCTIONS->glGetShaderiv(TargetShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            OPENGL_EXTRA_FUNCTIONS->glGetShaderInfoLog(TargetShader, 512, NULL, infoLog);
            // 检查是否包含自定义标志
            if (strstr(infoLog, "VERTEX_SHADER_FLAG") != nullptr)
            {
                Logger::error("SHADER::VERTEX::COMPILATION_FAILED");
            }
            else
            {
                Logger::error("SHADER::FRAGMENT::COMPILATION_FAILED");
            }
        }
    }
    else if (type == "LINK")
    {
        OPENGL_EXTRA_FUNCTIONS->glGetProgramiv(TargetShader, GL_LINK_STATUS, &success);
        if (!success)
        {
            OPENGL_EXTRA_FUNCTIONS->glGetProgramInfoLog(TargetShader, 512, NULL, infoLog);
            Logger::error("SHADER::PROGRAM::LINK_FAILED");
        }
    }
    else
    {
        Logger::error("SHADER::CHECK_ERROR::UNKNOWN_TYPE");
    }
}
