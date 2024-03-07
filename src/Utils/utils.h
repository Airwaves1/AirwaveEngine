#pragma once

#include <qopenglcontext.h>
#include <qopenglfunctions.h>
#include <qopenglextrafunctions.h>

#include "logger.h"

//#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

//检查OpenGL错误
#define CHECK_OPENGL_ERROR() \
do { \
        GLenum error = OPENGL_FUNCTIONS->glGetError(); \
        if (error != GL_NO_ERROR) { \
            Logger::error("OpenGL error during setup: %1 ", error); \
    } \
} while (false)



// OpenGL核心函数
#define OPENGL_FUNCTIONS QOpenGLContext::currentContext()->functions()
//OpenGL扩展函数
#define OPENGL_EXTRA_FUNCTIONS QOpenGLContext::currentContext()->extraFunctions()


