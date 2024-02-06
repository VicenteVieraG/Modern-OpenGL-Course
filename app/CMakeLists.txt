add_executable(${CMAKE_PROJECT_NAME} main.cpp)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE GLEW::GLEW
    PRIVATE glfw
)