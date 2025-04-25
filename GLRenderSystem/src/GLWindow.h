#pragma once
#define GLFW_INCLUDE_NONE
#include "glad.h"
#include "../../ThirdParty\glfw\include\GLFW\glfw3.h"
#include "../../Interfaces/IWindow.h"
#include <functional>


class GLWindow : public IWindow
{ 
   

public: 
GLWindow(uint32_t width, uint32_t height, const std::string& title);
~GLWindow() override;

uint32_t getWidth() const override;
uint32_t getHeight() const override;

void setKeyCallback(const KeyCallback& callback) override;
void setCursorPosCallback(const CursorPosCallback& callback) override;
void setMouseCallback(const MouseCallback& callback) override;
void setScrollCallback(const ScrollCallback& callback) override;


GLFWwindow* getNativeHandle() const override; 
private: 


GLFWwindow* handle = nullptr;
uint32_t width, height;

KeyCallback keyCallback;
CursorPosCallback cursorPosCallback;
MouseCallback mouseCallback;
ScrollCallback scrollCallback;

};
