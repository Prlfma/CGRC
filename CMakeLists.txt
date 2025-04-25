cmake_minimum_required(VERSION 3.16)
project(MeshEditorSolution)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ThirdParty/glfw)

add_subdirectory(GLRenderSystem)

add_subdirectory(MeshEditor)
