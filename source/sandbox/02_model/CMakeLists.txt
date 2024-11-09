cmake_minimum_required (VERSION 3.8)

project(02_model)


add_executable(02_model 
    "02_model.cpp"
)

target_link_libraries(02_model PRIVATE engine)
