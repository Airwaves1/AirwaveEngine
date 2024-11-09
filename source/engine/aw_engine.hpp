#pragma once



//---------------utils-------------------
#include "platform/utils/log.hpp"
#include "platform/utils/file_utils.hpp"
#include "platform/utils/geometry_utils.hpp"

//---------------event-------------------
#include "platform/event/event.hpp"
#include "platform/event/event_dispatcher.hpp"
#include "platform/event/event_observer.hpp"
#include "platform/event/application_event.hpp"
#include "platform/event/key_event.hpp"
#include "platform/event/mouse_event.hpp"


//---------------opengl-------------------
#include "platform/opengl/gl_shader.hpp"
#include "platform/opengl/gl_texture.hpp"
#include "platform/opengl/gl_vertex_array.hpp"
#include "platform/opengl/gl_vertex_buffer.hpp"
#include "platform/opengl/gl_cube_texture.hpp"

//---------------resource-------------------
#include "resource/shader_lib.hpp"
#include "resource/texture_lib.hpp"
#include "resource/obj_loader.hpp"

//---------------camera-------------------
#include "core/camera/camera.hpp"
#include "core/camera/controller/trackball_controller.hpp"

//---------------ecs-------------------
#include "core/ecs/aw_entity.hpp"
#include "core/ecs/system/aw_system.hpp"
#include "core/ecs/scene.hpp"
#include "core/ecs/uuid.hpp"

//---------------ecs component-------------------
#include "core/ecs/component/camera/camera_component.hpp"
#include "core/ecs/component/camera/camera_controller_component.hpp"
#include "core/ecs/component/material/material_component.hpp"
#include "core/ecs/component/mesh/mesh_component.hpp"
#include "core/ecs/component/transform/transform_component.hpp"
#include "core/ecs/component/light/light_component.hpp"


//---------------system-------------------
#include "core/ecs/system/camera/camera_system.hpp"
#include "core/ecs/system/render/forward_render_system.hpp"
#include "core/ecs/system/render/backgroud_system.hpp"

//---------------application-------------------
#include "core/application/application.hpp"
#include "core/application/entry_point.hpp"

