#include "rendering/primitive.hpp"

namespace Airwave
{
void Primitive::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Primitive::drawInstanced(uint32_t instanceCount)
{
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instanceCount);
    glBindVertexArray(0);
}

} // namespace Airwave
