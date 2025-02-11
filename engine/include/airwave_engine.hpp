#pragma once

#include "core/application.hpp"
#include "core/window.hpp"
#include "core/log.hpp"
#include "core/event/event.hpp"
#include "core/event/event_dispatcher.hpp"
#include "core/event/event_observer.hpp"

#include "core/math/plane.hpp"
#include "core/math/aabb.hpp"

#include "ecs/aw_scene.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/aw_system.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/tag_component.hpp"
#include "ecs/components/hierarchy_component.hpp"
#include "ecs/components/mesh_component.hpp"
#include "ecs/components/material_component.hpp"
#include "ecs/components/camera_component.hpp"
#include "ecs/components/light_component.hpp"

#include "ecs/components/singleton_components/input_component.hpp"
#include "ecs/components/singleton_components/lights_manager_component.hpp"
#include "ecs/components/singleton_components/renderer_component.hpp"

#include "ecs/systems/transform_system.hpp"
#include "ecs/systems/camera_system.hpp"
#include "ecs/systems/render_system.hpp"
#include "ecs/systems/input_system.hpp"


#include "rendering/renderer.hpp"   
#include "rendering/primitive.hpp"
#include "rendering/material.hpp"
#include "rendering/opengl/gl_shader.hpp"
#include "rendering/opengl/gl_texture.hpp"
#include "rendering/opengl/gl_vertex_array.hpp"
#include "rendering/opengl/gl_vertex_buffer.hpp"
#include "rendering/opengl/gl_framebuffer.hpp"

#include "utils/geometry_utils.hpp"
#include "utils/shapes_vao.hpp"
#include "utils/texture_utils.hpp"
#include "utils/file_utils.hpp"
#include "utils/shader_utils.hpp"

#include "resource/resource_manager.hpp"
#include "resource/resource.hpp"
#include "resource/texture_resource.hpp"
#include "resource/shader_resource.hpp"

