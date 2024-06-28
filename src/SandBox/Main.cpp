#include "Application/Application.h"
#include "Application/EntryPoint.h"

#include <glad/glad.h>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Utils/Common.h"
#include "Utils/Log.h"
#include "Utils/FileUtils.h"

#include "OpenGL/OpenGLContext.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"

#include "Geometry/Geometry.h"

#include "Camera/PerspectiveCamera.h"
#include "Controller/TrackBallCameraController.h"
#include "Application/ControllerEvent.h"

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
	}

	void OnInit() override
	{
		Airwave::OpenGLContext m_OpenGLContext = Airwave::OpenGLContext(GetWindow());
		m_OpenGLContext.Init();

		//创建相机和控制器
		m_Camera = std::make_unique<Airwave::PerspectiveCamera>(75.0f, float(GetWindowWidth()) / GetWindowHeight(), 0.1f, 100.0f);
		m_CameraController = std::make_shared<Airwave::TrackBallCameraController>(m_Camera.get());

		//设置事件
		Airwave::ControllerEvent::GetInstance()->SetEvent(m_CameraController.get(), GetWindow());

		//创建立方体对象
		m_Box = std::make_shared<Airwave::BoxGeometry>(1.0, 1.0, 1.0);

		//创建着色器
		std::string vertexShaderPath = ASSETS_SHADER_DIR "00/003_multitexture_cube.vert";
		std::string fragmentShaderPath = ASSETS_SHADER_DIR "00/003_multitexture_cube.frag";
		m_Shader = std::make_unique<Airwave::OpenGLShader>(vertexShaderPath, fragmentShaderPath);

		// Airwave::PrintFileInfoOnLog(vertexShaderPath);

		//创建纹理
		std::string texture0Path = ASSETS_TEXTURE_DIR "container2.png";
		std::string texture1Path = ASSETS_TEXTURE_DIR "container2_specular.png";

		m_Texture0 = std::make_unique<Airwave::OpenGLTexture>(texture0Path);
		m_Texture1 = std::make_unique<Airwave::OpenGLTexture>(texture1Path);

		m_Shader->Bind();
		m_Shader->UploadUniformInt("u_Texture0", 0);
		m_Shader->UploadUniformInt("u_Texture1", 1);
	
	}

	void OnUpdate(float deltaTime) override
	{
		glViewport(0, 0, GetWindowWidth(), GetWindowHeight());

		glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(m_Angle), glm::vec3(0.5f, 0.8f, 0.5f));
		view = m_Camera->GetViewMatrix();
		projection = m_Camera->GetProjectionMatrix();

		m_Shader->Bind();
		m_Shader->UploadUniformMat4("u_Model", model);
		m_Shader->UploadUniformMat4("u_View", view);
		m_Shader->UploadUniformMat4("u_Projection", projection);

		m_Angle += 0.1f;
		m_CameraController->Update(deltaTime);
	}

	void OnRender() override
	{
		m_Shader->Bind();


		m_Texture0->Bind(0);
		m_Texture1->Bind(1);

		m_Box->Show();


	
	}

	void OnDestroy() override
	{

	}

private:
	GLuint m_VAO;

	float m_Angle = 0.0f;

	//创建立方体对象
	std::shared_ptr<Airwave::Geometry> m_Box;

	std::unique_ptr<Airwave::OpenGLShader> m_Shader;
	std::unique_ptr<Airwave::OpenGLTexture> m_Texture0;
	std::unique_ptr<Airwave::OpenGLTexture> m_Texture1;

	//创建相机和控制器
	std::unique_ptr<Airwave::Camera> m_Camera;
	std::shared_ptr<Airwave::CameraController> m_CameraController;
};

Airwave::Application* CreateApplicationEntryPoint()
{
	return new SandBoxApp();
}

#include "Windows/GLFWWindow.h"