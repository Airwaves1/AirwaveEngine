#include "rendering/uniforms.hpp"

namespace Airwave
{
void GLUniforms::createUBO(const std::string &blockName, GLuint program, GLuint bindingPoint) {}

void GLUniforms::setUBOData(const std::string &blockName, const void *data, size_t size) {}

void GLUniforms::bindUBO(const std::string &blockName) {}

} // namespace Airwave
