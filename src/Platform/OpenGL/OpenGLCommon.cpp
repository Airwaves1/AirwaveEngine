#include "OpenGLCommon.h"

void ClearError() {
	while (glad_glGetError() != GL_NO_ERROR);
}


const char* GLErrorToString(GLenum errorCode) {
    switch (errorCode) {
    case GL_NO_ERROR:
        return "No error";
    case GL_INVALID_ENUM:
        return "Invalid enum";
    case GL_INVALID_VALUE:
        return "Invalid value";
    case GL_INVALID_OPERATION:
        return "Invalid operation";
    case GL_STACK_OVERFLOW:
        return "Stack overflow";
    case GL_STACK_UNDERFLOW:
        return "Stack underflow";
    case GL_OUT_OF_MEMORY:
        return "Out of memory";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "Invalid framebuffer operation";
    default:
        return "Unknown OpenGL error";
    }
}

bool LogCall(const char* function, const char* file, int line) {
    while (GLenum error = glad_glGetError()) {

        LOG_ERROR("OpenGL Error {0} : {1} {2} {3} {4}", error, GLErrorToString(error), function, file, line);

        return false;
    }
    return true;
}