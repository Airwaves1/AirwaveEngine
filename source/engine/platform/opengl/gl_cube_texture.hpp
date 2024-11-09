#pragma once

#include <array>
#include <string>

namespace Airwave
{
class CubeTexture
{
  public:
    CubeTexture(const std::array<std::string, 6> &faces);
    ~CubeTexture();

    void bind(uint32_t slot = 0) const;
    void unbind() const;

    uint32_t getHandle() const { return m_handle; }

    void loadTextures(const std::array<std::string, 6> &faces);
    void createEmptyTexture(uint32_t width, uint32_t height) {}

  private:
    uint32_t m_handle;

};

} // namespace Airwave
