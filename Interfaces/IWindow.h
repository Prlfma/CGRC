#pragma once

#include "../ThirdParty\glfw\include\GLFW\glfw3.h"
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
    DELETE = 46,
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

class IWindow
{ 
public:
using KeyCallback = std::function<void(KeyCode, Action, Modifier)>;
using CursorPosCallback = std::function<void(double, double)>;
using MouseCallback = std::function<void(ButtonCode, Action, Modifier, double, 
double)>;
using ScrollCallback = std::function<void(double, double)>;
virtual ~IWindow() {}
virtual uint32_t getWidth() const = 0;
virtual uint32_t getHeight() const = 0;
virtual void setKeyCallback(const KeyCallback& callback) = 0;
virtual void setCursorPosCallback(const CursorPosCallback& callback) = 0;
virtual void setMouseCallback(const MouseCallback& callback) = 0;
virtual void setScrollCallback(const ScrollCallback& callback) = 0;
virtual GLFWwindow* getNativeHandle() const = 0; 
};
