#pragma once
#include "application.hpp"

extern Airwave::Application *CreateApplication(int argc, char **argv);

int main(int argc, char **argv)
{
    Airwave::Application *app = CreateApplication(argc, argv);
    app->init(argc, argv);
    app->mainLoop();
    app->shutdown();
    delete app;
    return 0;
} // namespace Airwave
