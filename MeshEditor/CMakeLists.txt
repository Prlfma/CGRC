cmake_minimum_required(VERSION 3.15)
project(MeshEditor LANGUAGES C CXX)

# Вимкнути автоматичні Qt функції
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

# Налаштування стандартів C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW конфіг
set(GLFW_BUILD_WIN32 ON)
set(GLFW_BUILD_WAYLAND OFF)
set(GLFW_BUILD_X11 OFF)
set(GLFW_USE_NULL OFF)
set(GLFW_BUILD_NULL OFF CACHE BOOL "Disable GLFW Null platform" FORCE)

# Додати GLFW як підпроєкт
add_subdirectory(ThirdParty/glfw)

# Основний виконуваний файл
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/GLRenderSystem.cpp
    src/GLWindow.cpp
    src/Camera.cpp
    src/Viewport.cpp
    src/glad.c
    ../Parser/STLParser.cpp
)

# Макроси тільки для MeshEditor
target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLFW_INCLUDE_NONE
)

# Заголовки
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ThirdParty/glm
    ThirdParty/glfw/include
)

# Правильне лінкування з ім'ям таргету CMake — glfw, а не glfw3
target_link_libraries(${PROJECT_NAME}
    glfw
    opengl32
    gdi32
    user32
)

# Вимкнути попередження про перевизначення (для MinGW)
if(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-macro-redefined)
endif()
