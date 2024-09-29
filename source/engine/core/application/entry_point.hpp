#include "application.hpp"
#include <iostream>

extern Airwave::Application *CreateAirwaveEngineApplication();

int main(int argc, char **argv)
{
    std::cout << "Hello, Airwave Engine!" << std::endl;

    auto app = CreateAirwaveEngineApplication();

    app->start(argc, argv);

    app->mainLoop();

    app->quit();
    
    return EXIT_SUCCESS;
}