#pragma once



//---------------utils-------------------
#include "platform/utils/log.hpp"
#include "platform/utils/file_utils.hpp"

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

//---------------ecs-------------------
#include "core/ecs/aw_entity.hpp"
#include "core/ecs/system/aw_system.hpp"
#include "core/ecs/scene.hpp"
#include "core/ecs/uuid.hpp"

//---------------system-------------------
#include "core/ecs/system/camera/camera_system.hpp"

//---------------application-------------------
#include "core/application/application.hpp"
#include "core/application/entry_point.hpp"