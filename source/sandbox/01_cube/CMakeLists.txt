cmake_minimum_required (VERSION 3.8)

project(01_cube)


add_executable(01_cube 
    "01_cube.cpp"
)

target_link_libraries(01_cube PRIVATE engine)
