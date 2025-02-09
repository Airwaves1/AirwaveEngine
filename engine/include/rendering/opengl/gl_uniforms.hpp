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
        Float,
        Vec2,
        Vec3,
        Vec4,
        Int,
        IVec2,
        IVec3,
        IVec4,
        Mat3,
        Mat4,
        Bool
    };

    void set(const std::string &name, const UniformValue &value)
    {
        auto &uniform = m_uniforms[name];
        uniform.value = value;
        uniform.type  = std::visit([&](auto &&val) { return getType<decltype(val)>(); }, value);
        uniform.dirty = true;
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

    void Clear()
    {
        m_uniforms.clear();
        m_locationCache.clear();
    }

  private:
    struct UniformInfo
    {
        UniformValue value;
        DataType type;
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
        if (it != cache.end()) return it->second.location;

        GLint location = glGetUniformLocation(program, name.c_str());
        cache[name]    = {location, false};
        return location;
    }

    template <typename T> DataType getType()
    {
        if constexpr (std::is_same_v<T, float>)
            return DataType::Float;
        else if constexpr (std::is_same_v<T, glm::vec2>)
            return DataType::Vec2;
        else if constexpr (std::is_same_v<T, glm::vec3>)
            return DataType::Vec3;
        else if constexpr (std::is_same_v<T, glm::vec4>)
            return DataType::Vec4;
        else if constexpr (std::is_same_v<T, int>)
            return DataType::Int;
        else if constexpr (std::is_same_v<T, glm::ivec2>)
            return DataType::IVec2;
        else if constexpr (std::is_same_v<T, glm::ivec3>)
            return DataType::IVec3;
        else if constexpr (std::is_same_v<T, glm::ivec4>)
            return DataType::IVec4;
        else if constexpr (std::is_same_v<T, glm::mat3>)
            return DataType::Mat3;
        else if constexpr (std::is_same_v<T, glm::mat4>)
            return DataType::Mat4;
        else if constexpr (std::is_same_v<T, bool>)
            return DataType::Bool;
        else
        {
            LOG_ERROR("GLUniforms::getType: unsupported type");
            return DataType::Float;
        }
    }

    void uploadUniform(GLint location, const UniformInfo &uniform)
    {
        std::visit(
            [&](auto &&val)
            {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, float>)
                    glUniform1f(location, val);
                else if constexpr (std::is_same_v<T, glm::vec2>)
                    glUniform2fv(location, 1, glm::value_ptr(val));
                else if constexpr (std::is_same_v<T, glm::vec3>)
                    glUniform3fv(location, 1, glm::value_ptr(val));
                else if constexpr (std::is_same_v<T, glm::vec4>)
                    glUniform4fv(location, 1, glm::value_ptr(val));
                else if constexpr (std::is_same_v<T, int>)
                    glUniform1i(location, val);
                else if constexpr (std::is_same_v<T, glm::ivec2>)
                    glUniform2iv(location, 1, glm::value_ptr(val));
                else if constexpr (std::is_same_v<T, glm::ivec3>)
                    glUniform3iv(location, 1, glm::value_ptr(val));
                else if constexpr (std::is_same_v<T, glm::ivec4>)
                    glUniform4iv(location, 1, glm::value_ptr(val));
                else if constexpr (std::is_same_v<T, glm::mat3>)
                    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(val));
                else if constexpr (std::is_same_v<T, glm::mat4>)
                    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
                else if constexpr (std::is_same_v<T, bool>)
                    glUniform1i(location, val);
            },
            uniform.value);
    }
};

} // namespace Airwave
