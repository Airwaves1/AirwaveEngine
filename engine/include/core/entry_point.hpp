#pragma once
#include <iostream>
#include "application.hpp"

extern Airwave::Application* CreateAirwaveEngineApplication();

int main(int argc, char** argv)
{
    auto app = CreateAirwaveEngineApplication();

    app->start(argc, argv);

    app->mainLoop();

    app->quit();

    delete app;

    return EXIT_SUCCESS;
}