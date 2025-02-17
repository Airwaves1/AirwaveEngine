cmake_minimum_required(VERSION 3.8)

add_library(third_party INTERFACE)

add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
set(SPDLOG_FMT_EXTERNAL ON)

# glfw
add_subdirectory(glfw)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

# glad
add_subdirectory(glad)

# glm
option(GLM_ENABLE_EXPERIMENTAL ON)
option(GLM_ENABLE_CXX_20 "Enable C++ 20 features" ON)
add_subdirectory(glm)


# imgui
add_subdirectory(imgui)

# nlohmann_json
add_subdirectory(nlohmann_json)

# tiny_gltf
add_subdirectory(tiny_gltf)

#tiny_obj
add_subdirectory(tiny_obj)

# bullet
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Disable demos")
set(BUILD_BULLET3 ON CACHE BOOL "Core physics engine") 
set(BUILD_EXTRAS OFF CACHE BOOL "No extras")
set(BUILD_UNIT_TESTS OFF CACHE BOOL "No tests")
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "No GUI")

# 模块控制
set(BUILD_CPU_DEMOS OFF CACHE BOOL "")
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "")
set(BUILD_PYBULLET OFF CACHE BOOL "")

# 依赖管理
set(BT_USE_EGL OFF CACHE BOOL "")   
set(USE_X11 OFF CACHE BOOL "")      
set(USE_GLFW OFF CACHE BOOL "")     
set(ENABLE_VHACD OFF CACHE BOOL "") 

# 网络模块
set(BUILD_CLSOCKET OFF CACHE BOOL "")
set(BUILD_ENET OFF CACHE BOOL "")

# 多线程
set(BT_USE_OPENMP OFF CACHE BOOL "")   # 正确选项名称

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # 压制所有第三方库警告
    add_compile_options(-isystem ${CMAKE_SOURCE_DIR}/bullet3)
    
    # 或选择性关闭特定警告
    add_compile_options(
        -Wno-deprecated-volatile
        -Wno-single-bit-bitfield-constant-conversion
        -Wno-class-memaccess
        -Wno-invalid-offsetof
    )
elseif(MSVC)
    add_compile_options(/wd4267 /wd4244 /wd4305) # 压制C4267/C4244等
endif()

add_subdirectory(bullet3)


target_link_libraries(third_party INTERFACE glfw)
target_link_libraries(third_party INTERFACE glad)
target_link_libraries(third_party INTERFACE glm)
target_link_libraries(third_party INTERFACE ImGui)
target_link_libraries(third_party INTERFACE nlohmann_json)
target_link_libraries(third_party INTERFACE tiny_gltf)
target_link_libraries(third_party INTERFACE tiny_obj)
target_link_libraries(third_party INTERFACE BulletDynamics BulletCollision LinearMath)