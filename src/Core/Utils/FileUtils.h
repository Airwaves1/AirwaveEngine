#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "Common.h"


#ifdef ASSETS_DEFINE_ROOT_DIR
#define ASSETS_ROOT_DIR ASSETS_DEFINE_ROOT_DIR
#else
#define ASSETS_ROOT_DIR "assets/"
#endif


#define ASSETS_SHADER_DIR ASSETS_ROOT_DIR "Shader/"
#define ASSETS_MODEL_DIR ASSETS_ROOT_DIR "Model/"
#define ASSETS_TEXTURE_DIR ASSETS_ROOT_DIR "Texture/"

#endif // !FILE_UTILS_H
