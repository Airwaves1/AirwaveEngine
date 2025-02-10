#include <iostream>
#include <string>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <variant>

using namespace std;

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

using UniformsValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4>;

struct UniformInfo
{
    DataType type;
    UniformsValue value;
    bool dirty = true;
};

// 全局存储 Uniform 数据
unordered_map<string, UniformInfo> uniformStorage;

void set(const std::string &name, const UniformsValue &value)
{
    auto it = uniformStorage.find(name);
    if (it != uniformStorage.end())
    {
        it->second.value = value;
        it->second.dirty = true;
    }
    else
    {
        // 自动推导类型
        DataType type;
        if (std::holds_alternative<int>(value))
            type = DataType::Int;
        else if (std::holds_alternative<float>(value))
            type = DataType::Float;
        else if (std::holds_alternative<glm::vec2>(value))
            type = DataType::Vec2;
        else if (std::holds_alternative<glm::vec3>(value))
            type = DataType::Vec3;
        else if (std::holds_alternative<glm::vec4>(value))
            type = DataType::Vec4;
        else if (std::holds_alternative<glm::mat3>(value))
            type = DataType::Mat3;
        else
            type = DataType::Mat4;

        uniformStorage[name] = {type, value, true};
    }
}

// 统一打印 variant 值
void print(const UniformInfo &info)
{
    std::visit([](auto &&arg)
               {
                   using T = std::decay_t<decltype(arg)>;
                   if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float>)
                   {
                       std::cout << arg << std::endl;
                   }
                   else
                   {
                       std::cout << glm::to_string(arg) << std::endl;
                   }
               },
               info.value);
}

int main()
{
    set("u_Int", 42);
    set("u_Float", 3.14f);
    set("u_Vec3", glm::vec3(1.0f, 2.0f, 3.0f));
    set("u_Mat4", glm::mat4(1.0f));

    for (const auto &[name, uniform] : uniformStorage)
    {
        cout << "Uniform: " << name << " = ";
        print(uniform);
    }

    return 0;
}
