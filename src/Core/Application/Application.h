#ifndef APPLICATION_H
#define APPLICATION_H


#include "Utils/Common.h"
#include "Application/ApplicationContext.h"
#include <chrono>


namespace Airwave {

	//TODO : 抽象Window类

	struct AppSettings
	{
		uint32_t width{ 800 };
		uint32_t height{ 600 };
		std::string title{ "AirwaveEngine" };
	};

	class Window;


	class Application {

	public:

		static ApplicationContext* GetAppContext();

	public:
		Application() = default;
		virtual ~Application();

		void Start(int argc, char* argv[]);
		void Stop();
		void MainLoop();


		void Pause() { b_Pause = true; }
		void Resume() { if(b_Pause) {b_Pause = false; } }
		bool IsPaused() const { return b_Pause; }

		//获取窗口
		Window* GetWindow() { return m_Window.get(); }

		virtual uint32_t GetWindowWidth() const;
		virtual uint32_t GetWindowHeight() const;

	protected:
		virtual void OnConfiguration(AppSettings* appSettings) {}
		virtual void OnInit() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnDestroy() {}


	private:
		 
		void ParseArgs(int argc, char* argv[]);

	private:
		bool b_IsRunning{ false };
		AppSettings m_AppSettings;

		std::unique_ptr<Window> m_Window{ nullptr };

		std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
		std::chrono::time_point<std::chrono::steady_clock> m_LastTimePoint;

		float deltaTime{ 0.0f };
		uint64_t m_FrameIndex{ 0 };

		bool b_Pause{ false };

		static ApplicationContext s_AppContext;
	};

}


#endif // !APPLICATION_H
