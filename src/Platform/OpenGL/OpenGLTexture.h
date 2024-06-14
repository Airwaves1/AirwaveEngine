#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H


#include <glad/glad.h>
#include "Utils/Common.h"

namespace Airwave {

	struct TextureConfig {
		GLenum target = GL_TEXTURE_2D;           // 纹理目标，默认为GL_TEXTURE_2D
		GLenum internalFormat = GL_RGBA8;        // 纹理的内部格式
		GLenum type = GL_UNSIGNED_BYTE;          // 数据的类型
		GLenum wrapS = GL_REPEAT;                // S轴的环绕方式
		GLenum wrapT = GL_REPEAT;                // T轴的环绕方式
		GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;            // 缩小过滤器
		GLenum magFilter = GL_LINEAR;            // 放大过滤器
		bool mipMap = true;                      // 是否生成MipMap
		bool flip = true;                        // 是否翻转图片
		bool sRGB = false;                       // 是否使用sRGB纹理
		bool freeData = true;                    // 是否释放数据
	};



	class OpenGLTexture {
	public:
		OpenGLTexture(const std::string& path, const TextureConfig& config = TextureConfig());
		~OpenGLTexture();

		void Bind(unsigned int slot);
		void Unbind();

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

		uint32_t GetRendererID() const { return m_RendererID; }

	private:
		std::string m_Path;
		uint32_t m_RendererID;
		int m_Width, m_Height, m_BPP;
		unsigned char* m_LocalBuffer;

		TextureConfig m_Config;
	};

}


#endif // !OPENGL_TEXTURE_H
