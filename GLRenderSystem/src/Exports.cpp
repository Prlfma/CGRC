#include "Exports.h"
#include "GLWindow.h"
#include "GLRenderSystem.h"

#include "../../ThirdParty/glfw/include/GLFW/glfw3.h"

extern "C" OGL_RENDER_SYSTEM_API IRenderSystem* createRenderSystem() {
    return new GLRenderSystem();
}

extern "C" OGL_RENDER_SYSTEM_API IWindow* createWindow(const std::string& title, uint32_t width, uint32_t height) {
    static bool glfwInitialized = false;
    if (!glfwInitialized) {
        if (!glfwInit()) {
            return nullptr;
        }
        glfwInitialized = true;
    }

    return new GLWindow(width, height, title.c_str());
}

extern "C" OGL_RENDER_SYSTEM_API void waitEvents() {
    glfwWaitEvents();
}

extern "C" OGL_RENDER_SYSTEM_API void swapDisplayBuffers(IWindow* window) {
    GLWindow* glWindow = dynamic_cast<GLWindow*>(window);
    if (glWindow && glWindow->getNativeHandle()) {
        glfwSwapBuffers(glWindow->getNativeHandle());
    }
}

extern "C" OGL_RENDER_SYSTEM_API bool windowShouldClose(IWindow* window) {
    GLWindow* glWindow = dynamic_cast<GLWindow*>(window);
    if (glWindow && glWindow->getNativeHandle()) {
        return glfwWindowShouldClose(glWindow->getNativeHandle());
    }
    return true;
}
