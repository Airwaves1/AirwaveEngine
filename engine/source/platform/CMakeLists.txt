file(GLOB_RECURSE platform_sources
    *.cpp
    *.h
)
add_library(platform STATIC ${platform_sources})
target_link_libraries(platform PUBLIC external)