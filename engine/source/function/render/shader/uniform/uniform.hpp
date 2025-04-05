#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <variant>
#include "core/log/log.hpp"

namespace Airwave
{
class Uniform
{
  public:
    // 统一数据存储类型
    using UniformData = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4,
                                     std::vector<uint8_t> // 用于SSBO原始数据
                                     >;

    // 缓冲区类型标记
    enum class BufferType
    {
        SingleUniform,
        UBO,
        SSBO
    };

    // 统一设置接口
    void Set(const std::string &name, const UniformData &data) { updateSingleUniform(name, data); }
    // 批量提交更新
    void Commit(GLuint program)
    {
        // 处理传统Uniform
        for (auto &[name, uniform] : m_singleUniforms)
        {
            if (!uniform.dirty) continue;

            GLint loc = getLocation(program, name);
            if (loc == -1) continue;

            std::visit(
                [loc](auto &&arg)
                {
                    uploadSingle(loc, arg);
                },
                uniform.value);

            uniform.dirty = false;
        }

        // TODO: 处理UBO/SSBO提交
    }

  private:
    void updateSingleUniform(const std::string &name, const UniformData &data)
    {
        auto &uniform = m_singleUniforms[name];
        uniform.value = data;
        uniform.dirty = true;
    }

    GLint getLocation(GLuint program, const std::string &name)
    {
        auto &progCache = m_locationCache[program];
        if (auto it = progCache.find(name); it != end(progCache))
        {
            return it->second;
        }

        GLint loc = glGetUniformLocation(program, name.c_str());
        if (loc == -1)
        {
            LOG_WARN("Uniform '{}' not found in program {}", name, program);
        }
        progCache[name] = loc;
        return loc;
    }

    template <typename T> static void uploadSingle(GLint loc, const T &value)
    {
        if constexpr (std::is_same_v<T, int>)
        {
            glUniform1i(loc, value);
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            glUniform1f(loc, value);
        }
        else if constexpr (std::is_same_v<T, glm::vec2>)
        {
            glUniform2fv(loc, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::vec3>)
        {
            glUniform3fv(loc, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::vec4>)
        {
            glUniform4fv(loc, 1, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::mat3>)
        {
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        }
        else if constexpr (std::is_same_v<T, glm::mat4>)
        {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        }
    }

  private:
    struct SingleUniform
    {
        UniformData value;
        bool dirty = true;
    };

    // 单值Uniform存储
    std::unordered_map<std::string, SingleUniform> m_singleUniforms;
    // 位置缓存
    std::unordered_map<GLuint, std::unordered_map<std::string, GLint>> m_locationCache;
};
} // namespace Airwave
