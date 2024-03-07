#include "MyTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Utils/logger.h"

MyTexture::MyTexture(const std::string& path, unsigned int uint)
{
    m_Unit = uint;

    //1.stbimage读取图片
    int channels;	//通道数

    //--翻转y轴
    stbi_set_flip_vertically_on_load(true);

    //读取图片,并且获取图片数据,参数：图片路径，图片宽度，图片高度，图片通道数，图片数据
    unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);

    //检查图片是否读取成功
    if (data == nullptr)
    {
        Logger::error("MyTexture::MyTexture: failed to load texture %1", path.c_str());
    }

    //2.生成纹理并且激活单元绑定
    OPENGL_FUNCTIONS->glGenTextures(1, &m_Texture);
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0 + m_Unit);	//激活单元
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, m_Texture);

    //3. 传输纹理数据,参数：纹理目标，多级渐远纹理级别，纹理存储格式，纹理宽度，纹理高度，0，源图像格式，源图像数据类型，源图像数据
    OPENGL_FUNCTIONS->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //传输的同时，会开辟显存空间，所以可以释放CPU端的数据
    //所以这里可以释放data
    stbi_image_free(data);

    //4. 设置纹理过滤方式
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//缩小过滤
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//放大过滤

    //5. 设置纹理包裹方式
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	//s轴环绕方式u
    OPENGL_FUNCTIONS->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	//t轴环绕方式v

}

MyTexture::~MyTexture()
{
    if (m_Texture != 0)
    {
        OPENGL_FUNCTIONS->glDeleteTextures(1, &m_Texture);
    }
}

void MyTexture::bind()
{
    OPENGL_EXTRA_FUNCTIONS->glActiveTexture(GL_TEXTURE0 + m_Unit);	//激活单元
    OPENGL_FUNCTIONS->glBindTexture(GL_TEXTURE_2D, m_Texture);
}
