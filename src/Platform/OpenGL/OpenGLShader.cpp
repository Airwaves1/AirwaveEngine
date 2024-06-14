#include "OpenGL/OpenGLShader.h"
#include "Utils/Log.h"

#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#include "OpenGLCommon.h"


namespace Airwave {
    OpenGLShader::OpenGLShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
        std::string vertexShaderCode = ReadFile(vertexShaderPath);
        std::string fragmentShaderCode = ReadFile(fragmentShaderPath);

        const char* vShaderCode = vertexShaderCode.c_str();
        const char* fShaderCode = fragmentShaderCode.c_str();

        uint32_t vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");

        m_ShaderProgramID = glCreateProgram();
        glAttachShader(m_ShaderProgramID, vertex);
        glAttachShader(m_ShaderProgramID, fragment);
        glLinkProgram(m_ShaderProgramID);
        CheckCompileErrors(m_ShaderProgramID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_ShaderProgramID);
    }

    void OpenGLShader::Bind() const {
        glUseProgram(m_ShaderProgramID);
    }

    void OpenGLShader::UnBind() const {
        glUseProgram(0);
    }

    int OpenGLShader::GetUniformLocation(const std::string& name)
    {
        static std::unordered_map<std::string, int> locationCache;
        if (locationCache.find(name) != locationCache.end()) {
			return locationCache[name];
		}
        int location = glGetUniformLocation(m_ShaderProgramID, name.c_str());
        if (location == -1) {
			LOG_WARN("Warning: uniform '{0}' doesn't exist!", name);
		}
		locationCache[name] = location;
		return location;
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
        CALL_GL(glUniform1i(GetUniformLocation(name), value));
	}

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
        CALL_GL(glUniform1f(GetUniformLocation(name), value));
	}

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
        CALL_GL(glUniform2f(GetUniformLocation(name), value.x, value.y));
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
		CALL_GL(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
	}

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
        CALL_GL(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
        CALL_GL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
        CALL_GL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }






    const std::string OpenGLShader::ReadFile(const std::string& filePath) {
        std::ifstream shaderFile(filePath);
        if (!shaderFile.is_open()) {
            LOG_ERROR("Error: Unable to open file: {0}", filePath);
            return "";
        }

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }

    void OpenGLShader::CheckCompileErrors(uint32_t shader, const std::string& type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                LOG_ERROR("Error: Shader compilation error of type: {0}, {1}", type, infoLog);
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                LOG_ERROR("Error: Program linking error of type: {0}, {1}", type, infoLog);
            }
        }
    }
}
