cmake_minimum_required(VERSION 3.22)
set (SANDBOX_SOURCE_FILE
	"Main.cpp"
)


add_executable(SandBox ${SANDBOX_SOURCE_FILE})


target_link_libraries(SandBox PRIVATE Core)