#pragma once
#define GLFW_INCLUDE_NONE
#include "../ThirdParty\glfw\include\GLFW\glfw3.h"
#include "../include/glad.h"

#include <functional>

enum class Modifier
{ 
NoModifier = 0,
Shift = 1,
Control = 2,
Alt = 4,
Super = 8,
};
enum class Action
{ 
Release = 0,
Press = 1,
Repeat = 2,
};
enum class ButtonCode
{ 
    Left   = 0,
    Right  = 1,
    Middle = 2,
};
enum class KeyCode
{ 
UNKNOWN = -1,
UP    = 265,  // GLFW_KEY_UP
    DOWN  = 264,  // GLFW_KEY_DOWN
    LEFT  = 263,  // GLFW_KEY_LEFT
    RIGHT = 262,  // GLFW_KEY_RIGHT
    W     = 87,
    A     = 65,
    S     = 83,
    D     = 68,
    SPACE = 32,
    ESC   = 256 ,
Space = 32,

R     = 82,  // Red +
T     = 84,  // Red -
G     = 71,  // Green +
H     = 72,  // Green -
B     = 66,  // Blue +
N     = 78,  // Blue -

NUM_1 = 49,
NUM_2 = 50,
NUM_3 = 51,
F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297
};
class GLWindow
{ 

public: 
using KeyCallback = std::function<void(KeyCode, Action, Modifier)>;
using CursorPosCallback = std::function<void(double, double)>;
using MouseCallback = std::function<void(ButtonCode, Action, Modifier, double, double)>;
using ScrollCallback = std::function<void(double, double)>;
GLWindow(const std::string& title, uint32_t width, uint32_t height);
~GLWindow();
uint32_t getWidth() const; 
uint32_t getHeight() const; 
void setKeyCallback(const KeyCallback& callback);
void setCursorPosCallback(const CursorPosCallback& callback);
void setMouseCallback(const MouseCallback& callback);
void setScrollCallback(const ScrollCallback& callback);
GLFWwindow* getGLFWHandle() const; 
private: 
GLFWwindow* handle = nullptr;
uint32_t width, height;

KeyCallback keyCallback;
CursorPosCallback cursorPosCallback;
MouseCallback mouseCallback;
ScrollCallback scrollCallback;

};
