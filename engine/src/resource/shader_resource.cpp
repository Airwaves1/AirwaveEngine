#include "resource/shader_resource.hpp"

#include <glad/glad.h>

#include "core/log.hpp"

#include "utils/file_utils.hpp"
#include "utils/shader_utils.hpp"

namespace Airwave
{
    
bool Airwave::ShaderResource::onLoad(const std::string &path, const std::any &params)
{
    if (!FileUtils::Exists(path))
    {
        LOG_ERROR("Shader file {0} does not exist", path);
        return false;
    }

    // 处理shader文件
    std::string source = ShaderUtils::GetInstance().process(path);

    // 分离顶点着色器和片段着色器
    auto [vertexSrc, fragmentSrc] = ShaderUtils::GetInstance().separateShader(source);

    m_shader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

    return true;
}

} // namespace Airwave
