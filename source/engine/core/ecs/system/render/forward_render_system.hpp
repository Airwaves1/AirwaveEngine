#pragma once

#include "ecs/system/aw_system.hpp"
#include <glad/glad.h>

namespace Airwave
{
    class ForwardRenderSystem : public AwSystem
    {
        public:
            ForwardRenderSystem() {}
            ~ForwardRenderSystem() {}

            void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) override
            {
                glClear(GL_COLOR_BUFFER_BIT);
            }
            
    };
    
} // namespace Airwave
