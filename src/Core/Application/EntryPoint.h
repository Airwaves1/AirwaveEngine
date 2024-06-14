#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H


#include "Application.h"

extern Airwave::Application* CreateApplicationEntryPoint();



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


#endif // !ENTRYPOINT_H
