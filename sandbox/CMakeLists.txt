cmake_minimum_required(VERSION 3.8)

add_executable(sandbox
    "sandbox.cpp"
)

target_link_libraries(sandbox PRIVATE engine)