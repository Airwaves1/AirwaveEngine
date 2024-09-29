#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>
namespace Airwave
{
enum class ShaderType
{
    NONE     = 0,
    VERTEX   = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER
};

class Shader
{
  public:
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc,
            bool fromFile = true,const std::vector<std::string> &defines = std::vector<std::string>());
    ~Shader();

    void bind() const;
    void unbind() const;

    void compile(const std::string &vertexSrc, const std::string &fragmentSrc,
                 bool fromFile = false, const std::vector<std::string> &defines = std::vector<std::string>());

    void recomplie();

    void addDefine(const std::vector<std::string> &defines);

    void removeDefine(const std::string &define);

    uint32_t getHandle() const { return m_handle; }

    uint32_t compileShader(const std::string &source, ShaderType type);

  public:
    uint32_t getUniformLocation(const std::string &name);
    void uploadUniformBool(const std::string &name, bool value);
    void uploadUniformInt(const std::string &name, int value);
    void uploadUniformInt2(const std::string &name, const glm::ivec2 &value);
    void uploadUniformFloat(const std::string &name, float value);
    void uploadUniformFloat2(const std::string &name, const glm::vec2 &value);
    void uploadUniformFloat3(const std::string &name, const glm::vec3 &value);
    void uploadUniformFloat4(const std::string &name, const glm::vec4 &value);
    void uploadUniformMat3(const std::string &name, const glm::mat3 &matrix);
    void uploadUniformMat4(const std::string &name, const glm::mat4 &matrix);


  private:
    uint32_t m_handle;

    std::vector<std::string> m_defines;

    std::unordered_map<ShaderType, std::string> m_sources;

    std::unordered_map<std::string, int> m_uniformLocationsCache;

    std::unordered_map<std::string, bool> m_uniformErrordCache;

    bool needsUpdate = false;
};

} // namespace Airwave
