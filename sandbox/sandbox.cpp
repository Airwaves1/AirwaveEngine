#include "airwave_engine.hpp"

namespace Airwave
{

class Sandbox : public Application
{
  public:
};
} // namespace Airwave


Airwave::Application *CreateWaveEngineApplication() { return new Airwave::Sandbox(); }
