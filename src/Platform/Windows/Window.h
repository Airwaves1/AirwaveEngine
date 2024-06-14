#ifndef WINDOW_H
#define WINDOW_H

#include <memory>

namespace Airwave {

	class Window {
	public:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		virtual ~Window() = default;

		static std::unique_ptr<Window> Create(uint32_t width = 800, uint32_t height = 600, const char* title = "");

		virtual bool ShouldClose() = 0;	
		virtual void PollEvents() = 0;	//设置回调函数
		virtual void SwapBuffers() = 0;	//双缓冲交换
		
	protected:
		Window() = default;

	};


}



#endif // WINDOW_H