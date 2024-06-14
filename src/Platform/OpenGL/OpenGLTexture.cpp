#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Utils/Log.h"
#include "OpenGLCommon.h"

namespace Airwave {

	OpenGLTexture::OpenGLTexture(const std::string& path, const TextureConfig& config)
		: m_Path(path), m_Config(config)
	{
		
		stbi_set_flip_vertically_on_load(m_Config.flip);

		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);
		if (!m_LocalBuffer) {
			LOG_ERROR("Failed to load image: {0}", path);
			return;
		}

		CALL_GL(glGenTextures(1, &m_RendererID));
		CALL_GL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Config.wrapS));
		CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Config.wrapT));

		CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Config.minFilter));
		CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Config.magFilter));


		GLenum format = GL_RGB;
		if (m_BPP == 4) {
			format = GL_RGBA;
		}

		CALL_GL(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, m_Config.type, m_LocalBuffer));
		
		if(m_Config.mipMap)
			CALL_GL(glGenerateMipmap(GL_TEXTURE_2D));

		if (m_Config.freeData)
			stbi_image_free(m_LocalBuffer);

		CALL_GL(glBindTexture(GL_TEXTURE_2D, 0));
					
	}

	OpenGLTexture::~OpenGLTexture() {
		glDeleteTextures(1, &m_RendererID);

	}


	void OpenGLTexture::Bind(unsigned int slot) {
		
		CALL_GL(glActiveTexture(GL_TEXTURE0 + slot));
		CALL_GL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	}

	void OpenGLTexture::Unbind() {
		CALL_GL(glBindTexture(GL_TEXTURE_2D, 0));
	}



}