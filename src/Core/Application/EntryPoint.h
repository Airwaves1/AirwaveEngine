#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H


#include "Application.h"

extern Airwave::Application* CreateApplicationEntryPoint();


#ifdef PLATFORM_WIN32 || PLATFORM_LINUX || PLATFORM_MACOS

int main(int argc, char** argv) {

	std::cout << "Airwave Engine starting ..." << std::endl;

	Airwave::Application* app = CreateApplicationEntryPoint();
	
	//start
	app->Start(argc,argv);

	//main loop
	app->MainLoop();
	
	//stop
	app->Stop();
	
	//delete
	delete app;

	return EXIT_SUCCESS;

}

#endif // PLATFORM_WIN32 || PLATFORM_LINUX || PLATFORM_MACOS





#endif // !ENTRYPOINT_H
