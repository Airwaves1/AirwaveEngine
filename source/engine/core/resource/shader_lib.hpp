#pragma once

#include <unordered_map>
#include <memory>
#include "opengl/gl_shader.hpp"
#include "utils/log.hpp"
#include "utils/file_utils.hpp"

#define SHADER_LIB Airwave::ShaderLibrary::getInstance()

namespace Airwave
{
class ShaderLibrary
{
  public:
    static ShaderLibrary &getInstance()
    {
        static ShaderLibrary instance;
        return instance;
    }

    void add(const std::string &name, const std::shared_ptr<Shader> &shader);

    std::shared_ptr<Shader> load(const std::string &name, const std::string &vertexPath,
                                 const std::string &fragmentPath, bool fromFile = true);

    std::shared_ptr<Shader> load(const std::string &name, const std::string &filePath);

    std::shared_ptr<Shader> get(const std::string &name);

    void remove(const std::string &name);

  private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};

} // namespace Airwave
