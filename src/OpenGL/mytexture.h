#include "Utils/utils.h"
#include <string>

class MyTexture
{
public:
    MyTexture(const std::string& path, unsigned int uint);
    ~MyTexture();

    void bind();

private:
    GLuint m_Texture{ 0 };

    int m_Width{ 0 };
    int m_Height{ 0 };
    unsigned int m_Unit{ 0 };

};
