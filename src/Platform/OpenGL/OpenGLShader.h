#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H


#include "Utils/Common.h"
#include <glm/glm.hpp>


namespace Airwave {

	class OpenGLShader {
	public:
		OpenGLShader(const std::string& vertexShader, const std::string& fragmentShader);
		~OpenGLShader();

		uint32_t GetShaderProgramID() const { return m_ShaderProgramID; }

		void Bind() const;
		void UnBind() const;


	public:

		int GetUniformLocation(const std::string& name);

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);


	private:
		const std::string ReadFile(const std::string& filePath);
		void CheckCompileErrors(const uint32_t shader, const std::string& type);
	private:
		uint32_t m_ShaderProgramID = 0;

	};
}

#endif // OPENGL_SHADER_H
