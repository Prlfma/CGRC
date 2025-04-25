project(GLRenderSystem)

add_library(GLRenderSystem SHARED
    src/Exports.cpp
    src/GLWindow.cpp
    src/GLRenderSystem.cpp
    src/Exports.h
    src/glad.c
    ../Interfaces/IWindow.h
    ../Interfaces/IRenderSystem.h
)

target_include_directories(GLRenderSystem
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/../Interfaces
)

target_link_libraries(GLRenderSystem
    PRIVATE glfw 
)

# Define for DLL export
target_compile_definitions(GLRenderSystem
    PRIVATE OGL_RENDER_SYSTEM_EXPORT
)
