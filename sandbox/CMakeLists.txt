cmake_minimum_required(VERSION 3.8)

add_executable(sandbox
    "sandbox.cpp"
)

target_link_libraries(sandbox PRIVATE engine editor)

add_subdirectory(editor)
add_subdirectory(game)