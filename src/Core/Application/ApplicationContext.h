#ifndef APPLICATION_CONTEXT_H
#define APPLICATION_CONTEXT_H


namespace Airwave {
	class Application;
	class Scene;
	class RenderContext;


	struct ApplicationContext {
		Application* app;
		Scene* scene;
		RenderContext* renderContext;
	};


}

#endif // !APPLICATION_CONTEXT_H
