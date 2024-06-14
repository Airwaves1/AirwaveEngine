#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Utils/Log.h"
#include "EventManager.h"

#include "Windows/Window.h"

namespace Airwave {

	ApplicationContext Application::s_AppContext{};


	ApplicationContext* Application::GetAppContext()
	{
		return &s_AppContext;
	}

	Application::~Application()
	{
		LOG_INFO("Application is destroyed");
	}

	void Application::Start(int argc, char* argv[])
	{
		Log::Init();

		ParseArgs(argc, argv);
		OnConfiguration(&m_AppSettings);


		m_Window = Window::Create(m_AppSettings.width, m_AppSettings.height, m_AppSettings.title.c_str());
		
		OnInit();

		m_StartTimePoint = std::chrono::steady_clock::now();

	}

	void Application::Stop()
	{
		OnDestroy();
	}
	 
	void Application::MainLoop()
	{
		m_LastTimePoint = std::chrono::steady_clock::now();

		while (!m_Window->ShouldClose())
		{
			m_Window->PollEvents();

			deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - m_LastTimePoint).count();

			m_LastTimePoint = std::chrono::steady_clock::now();

			if (!b_Pause) {
				OnUpdate(deltaTime);
			}

			OnRender();

			m_Window->SwapBuffers();
		}
	}



	void Application::ParseArgs(int argc, char* argv[])
	{
		//TODO
	}

}

