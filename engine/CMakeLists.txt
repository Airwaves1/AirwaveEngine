cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE core_source CONFIGURE_DEPENDS "src/core/*.cpp")
file(GLOB_RECURSE ecs_source CONFIGURE_DEPENDS "src/ecs/*.cpp")
file(GLOB_RECURSE rendering_source CONFIGURE_DEPENDS "src/rendering/*.cpp")
file(GLOB_RECURSE physics_source CONFIGURE_DEPENDS "src/physics/*.cpp")
file(GLOB_RECURSE utils_source CONFIGURE_DEPENDS "src/utils/*.cpp")

add_library(core STATIC ${core_source})
# add_library(ecs STATIC ${ecs_source})
# add_library(rendering STATIC ${rendering_source})
# add_library(physics STATIC ${physics_source})
# add_library(utils STATIC ${utils_source})

add_subdirectory(third_party)

add_library(engine INTERFACE)
target_link_libraries(engine INTERFACE third_party)
target_link_libraries(engine INTERFACE core)
# target_link_libraries(engine INTERFACE core ecs rendering physics utils)
