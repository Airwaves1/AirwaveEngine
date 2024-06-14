#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "Common.h"


#ifdef ASSETS_DEFINE_ROOT_DIR
#define ASSETS_ROOT_DIR ASSETS_DEFINE_ROOT_DIR
#else
#define ASSETS_ROOT_DIR "assets/"
#endif


#define ASSETS_SHADER_DIR ASSETS_ROOT_DIR "shader/"
#define ASSETS_MODEL_DIR ASSETS_ROOT_DIR "model/"
#define ASSETS_TEXTURE_DIR ASSETS_ROOT_DIR "texture/"

#endif // !FILE_UTILS_H
