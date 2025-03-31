#pragma once

#include "engine.hpp"

class Sandbox : public Airwave::Application
{

}
;
Airwave::Application *CreateApplication(int argc, char **argv)
{
    return new Sandbox();
} // namespace Airwave