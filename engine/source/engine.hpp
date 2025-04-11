#pragma once
#include "core/application/application.hpp"
#include "core/application/entry_point.hpp"
#include "core/log/log.hpp"
#include "core/uuid/uuid.hpp"

#include "function/global/global_context.hpp"
#include "function/window/aw_window.hpp"
#include "function/ecs/world.hpp"
#include "function/ecs/system.hpp"
#include "function/ecs/base_component.hpp"
#include "function/ecs/component/transform_component.hpp"
#include "function/ecs/component/camera_component.hpp"
#include "function/ecs/component/mesh_component.hpp"
#include "function/ecs/component/first_person_controller.hpp"

#include "function/render/texture/texture.hpp"
#include "function/render/shader/shader.hpp"
#include "function/render/shader/shader_preprocessor.hpp"