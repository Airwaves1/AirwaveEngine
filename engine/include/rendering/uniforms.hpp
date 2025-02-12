#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <variant>
#include "core/log.hpp"
#include "core/common.hpp"

namespace Airwave
{

class GLUniforms
{
  public:
    enum class DataType
    {
        Int,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat3,
        Mat4
    };

    void set(const std::string &name, const UniformValue &value)
    {
        auto it = m_uniforms.find(name);
        if (it != m_uniforms.end())
        {
            it->second.value = value;
            it->second.dirty = true; // 需要重新上传
        }
        else
        {
            m_uniforms[name] = {getType(value), value, true};
        }
    }

    void upload(GLuint program)
    {
        for (auto &[name, uniform] : m_uniforms)
        {
            if (!uniform.dirty) continue;

            GLint location = getCachedLocation(program, name);
            if (location == -1) continue;

            uploadUniform(location, uniform);
            uniform.dirty = false;
        }
    }

    void clear()
    {
        m_uniforms.clear();
        m_locationCache.clear();
    }

  private:
    struct UniformInfo
    {
        DataType type;
        UniformValue value;
        bool dirty = true;
    };

    struct LocationInfo
    {
        GLint location   = -1;
        bool hasReported = false;
    };

    std::unordered_map<std::string, UniformInfo> m_uniforms;
    std::unordered_map<GLuint, std::unordered_map<std::string, LocationInfo>> m_locationCache;

    GLint getCachedLocation(GLuint program, const std::string &name)
    {
        auto &cache = m_locationCache[program];
        auto it     = cache.find(name);
        if (it != cache.end())
        {
            return it->second.location;
        }

        GLint location = glGetUniformLocation(program, name.c_str());
        if (location == -1 && !cache[name].hasReported)
        {
            LOG_WARN("Uniform '{0}' not found in shader program [{1}]!", name, program);
            cache[name] = {location, true}; // 仅报告一次
        }
        else
        {
            cache[name] = {location, false}; // 位置有效，存入缓存
        }

        return location;
    }

    static DataType getType(const UniformValue &value)
    {
        if (std::holds_alternative<int>(value)) return DataType::Int;
        if (std::holds_alternative<size_t>(value)) return DataType::Int;
        if (std::holds_alternative<float>(value)) return DataType::Float;
        if (std::holds_alternative<glm::vec2>(value)) return DataType::Vec2;
        if (std::holds_alternative<glm::vec3>(value)) return DataType::Vec3;
        if (std::holds_alternative<glm::vec4>(value)) return DataType::Vec4;
        if (std::holds_alternative<glm::mat3>(value)) return DataType::Mat3;
        if (std::holds_alternative<glm::mat4>(value)) return DataType::Mat4;

        return DataType::Int;
    }

    static void uploadUniform(GLint location, const UniformInfo &info)
    {
        std::visit(
            [location](auto &&arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>)
                    glUniform1i(location, arg);
                else if constexpr (std::is_same_v<T, float>)
                    glUniform1f(location, arg);
                else if constexpr (std::is_same_v<T, glm::vec2>)
                    glUniform2fv(location, 1, glm::value_ptr(arg));
                else if constexpr (std::is_same_v<T, glm::vec3>)
                    glUniform3fv(location, 1, glm::value_ptr(arg));
                else if constexpr (std::is_same_v<T, glm::vec4>)
                    glUniform4fv(location, 1, glm::value_ptr(arg));
                else if constexpr (std::is_same_v<T, glm::mat3>)
                    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(arg));
                else if constexpr (std::is_same_v<T, glm::mat4>)
                    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(arg));
            },
            info.value);
    }
};

} // namespace Airwave
