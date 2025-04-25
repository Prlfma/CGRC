# cmake_minimum_required(VERSION 3.15)
# project(MeshEditor LANGUAGES C CXX)

# # Вимкнути автоматичні Qt функції
# set(CMAKE_AUTOMOC OFF)
# set(CMAKE_AUTORCC OFF)
# set(CMAKE_AUTOUIC OFF)

# # Налаштування стандартів C++
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # GLFW конфіг
# set(GLFW_BUILD_WIN32 ON)
# set(GLFW_BUILD_WAYLAND OFF)
# set(GLFW_BUILD_X11 OFF)
# set(GLFW_USE_NULL OFF)
# set(GLFW_BUILD_NULL OFF CACHE BOOL "Disable GLFW Null platform" FORCE)

project(MeshEditor)

add_executable(MeshEditor
    src/main.cpp
    src/DynamicLibrary.cpp
    src/HalfEdge.cpp
    src/Mesh.cpp
    src/Camera.cpp
    src/Viewport.cpp
    ../Parser/STLParser.cpp
)

target_include_directories(MeshEditor
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/../Interfaces
)

target_link_libraries(MeshEditor
    PRIVATE glfw
)

# For DynamicLibrary Windows support
target_compile_definitions(MeshEditor
    PRIVATE PLATFORM=PLATFORM_WIN32
)
