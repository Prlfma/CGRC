#include "GLWindow.h"
#include <iostream>

GLWindow::GLWindow(uint32_t width, uint32_t height, const std::string& title)
    : width(width), height(height)
{
    handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!handle) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(handle);

    static bool initGLAD = false;
    if (!initGLAD) {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }
        initGLAD = true;
    }

    glfwSetWindowUserPointer(handle, this);

    glfwSetKeyCallback(handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        GLWindow* win = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
        if (win && win->keyCallback) {
            win->keyCallback(static_cast<KeyCode>(key), static_cast<Action>(action), static_cast<Modifier>(mods));
        }
    });

    glfwSetCursorPosCallback(handle, [](GLFWwindow* window, double xpos, double ypos) {
        GLWindow* win = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
        if (win && win->cursorPosCallback) {
            win->cursorPosCallback(xpos, ypos);
        }
    });

    glfwSetMouseButtonCallback(handle, [](GLFWwindow* window, int button, int action, int mods) {
        GLWindow* win = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
        if (win && win->mouseCallback) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            win->mouseCallback(static_cast<ButtonCode>(button), static_cast<Action>(action), static_cast<Modifier>(mods), xpos, ypos);
        }
    });

    glfwSetScrollCallback(handle, [](GLFWwindow* window, double xoffset, double yoffset) {
        GLWindow* win = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
        if (win && win->scrollCallback) {
            win->scrollCallback(xoffset, yoffset);
        }
    });
}

GLWindow::~GLWindow()
{
    if (handle) {
        glfwDestroyWindow(handle);
    }
}

uint32_t GLWindow::getWidth() const
{
    return width;
}

uint32_t GLWindow::getHeight() const
{
    return height;
}

void GLWindow::setKeyCallback(const KeyCallback& callback)
{
    keyCallback = callback;
}

void GLWindow::setCursorPosCallback(const CursorPosCallback& callback)
{
    cursorPosCallback = callback;
}

void GLWindow::setMouseCallback(const MouseCallback& callback)
{
    mouseCallback = callback;
}

void GLWindow::setScrollCallback(const ScrollCallback& callback)
{
    scrollCallback = callback;
}

GLFWwindow* GLWindow::getNativeHandle() const
{
    return handle;
}
