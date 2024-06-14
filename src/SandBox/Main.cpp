#include "Application/Application.h"
#include "Application/EntryPoint.h"

#include <glad/glad.h>


#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Utils/Common.h"
#include "Utils/Log.h"


#include "OpenGL/OpenGLContext.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"

#include "Data.h"
#include "Utils/FileUtils.h"

class SandBoxApp : public Airwave::Application
{
public:
	~SandBoxApp() {}
	
protected:
	void OnConfiguration(Airwave::AppSettings* appSettings) override
	{
		appSettings->width = 1280;
		appSettings->height = 1120;
		appSettings->title = "SandBox";

		m_Width = appSettings->width;
		m_Height = appSettings->height;

	}

	void OnInit() override
	{
		Airwave::OpenGLContext m_OpenGLContext = Airwave::OpenGLContext(GetWindow());
		m_OpenGLContext.Init();

		//创建立方体
		GLuint VBO;
		
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//创建着色器
		std::string vertexShaderPath = ASSETS_SHADER_DIR "00/003_multitexture_cube.vert";
		std::string fragmentShaderPath = ASSETS_SHADER_DIR "00/003_multitexture_cube.frag";
		m_Shader = std::make_unique<Airwave::OpenGLShader>(vertexShaderPath, fragmentShaderPath);


		//创建纹理
		std::string texture0Path = ASSETS_TEXTURE_DIR "R-C.jpeg";
		std::string texture1Path = ASSETS_TEXTURE_DIR "awesomeface.png";

		m_Texture0 = std::make_unique<Airwave::OpenGLTexture>(texture0Path);
		m_Texture1 = std::make_unique<Airwave::OpenGLTexture>(texture1Path);

		m_Shader->Bind();
		m_Shader->UploadUniformInt("u_Texture0", 0);
		m_Shader->UploadUniformInt("u_Texture1", 1);
	
	}

	void OnUpdate(float deltaTime) override
	{
		glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(m_Angle), glm::vec3(0.5f, 0.8f, 0.5f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), m_Width / m_Height, 0.1f, 100.0f);

		m_Shader->Bind();
		m_Shader->UploadUniformMat4("u_Model", model);
		m_Shader->UploadUniformMat4("u_View", view);
		m_Shader->UploadUniformMat4("u_Projection", projection);



	}

	void OnRender() override
	{
		m_Shader->Bind();


		m_Texture0->Bind(0);
		m_Texture1->Bind(1);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		m_Angle += 0.1f;
	
	}

	void OnDestroy() override
	{

	}

	

private:
	GLuint m_VAO;

	std::unique_ptr<Airwave::OpenGLShader> m_Shader;
	std::unique_ptr<Airwave::OpenGLTexture> m_Texture0;
	std::unique_ptr<Airwave::OpenGLTexture> m_Texture1;

	float m_Angle = 0.0f;

	float m_Width { 1280 };
	float m_Height{ 1120 };
};

Airwave::Application* CreateApplicationEntryPoint()
{
	return new SandBoxApp();
}

#include "Windows/GLFWWindow.h"