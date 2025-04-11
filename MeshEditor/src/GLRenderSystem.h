#pragma once
#include "../ThirdParty\glm/glm.hpp"
#include "../ThirdParty\glfw\include\GLFW\glfw3.h"
#include <vector>

struct Vertex
{ 
glm::vec3 position;
glm::vec3 normal;
};

enum class RenderMode {
    Triangles,
    Edges,
    Vertices
};
class GLRenderSystem
{ 
public: 
void init(); //must be called after glfw window creation. Set default GL 

void clearDisplay(float r, float g, float b);
void setViewport(double x, double y, double width, double height);
void renderTriangleSoup(const std::vector<Vertex>& vertices,const glm::vec3& matColor);
void setupLight(uint32_t index, glm::vec3 position, glm::vec3 Ia, glm::vec3 Id, 
glm::vec3 Is);
void turnLight(uint32_t index, bool enable);
void setWorldMatrix(const glm::mat4& matrix);
const glm::mat4& getWorldMatrix() const; 
void setViewMatrix(const glm::mat4& matrix);
const glm::mat4& getViewMatrix() const; 
void setProjMatrix(const glm::mat4& matrix);
const glm::mat4& getProjMatrix() const; 
void setRenderMode(RenderMode mode) ;
const RenderMode getRenderMode() const;
private: 
// TODO
};
