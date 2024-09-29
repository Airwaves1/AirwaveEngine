cmake_minimum_required (VERSION 3.8)

project(00_triangle)


add_executable(00_triangle 
    "00_triangle.cpp"
)

target_link_libraries(00_triangle PRIVATE engine)
