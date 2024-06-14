#ifndef OPENGL_COMMON_H
#define OPENGL_COMMON_H

#include <glad/glad.h>
#include <cassert>
#include "Utils/Log.h"


void ClearError();

const char* GLErrorToString(GLenum errorCode);

bool LogCall(const char* function, const char* file, int line);

#define CALL_GL(x) do {\
	ClearError(); \
	x; \
    if (!LogCall(#x, __FILE__, __LINE__)) throw std::runtime_error("OpenGL error occurred."); \
} while(0) 

//if (!LogCall(#x, __FILE__, __LINE__)) {
//    \
//        std::cerr << "Fatal error encountered, terminating application." << std::endl; \
//        std::exit(EXIT_FAILURE); \
//}\


#endif // !OPENGL_COMMON_H