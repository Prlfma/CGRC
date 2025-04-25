#pragma once

#include <vector>
#include "../ThirdParty/glm/glm.hpp"
struct VertexSimple
{ 
glm::vec3 position;
glm::vec3 normal;
};

enum class RenderMode {
    Triangles,
    Edges,
    Vertices
};
class IRenderSystem
{ 
    public:
    virtual ~IRenderSystem() = default;
    virtual void init() = 0;
    virtual void clearDisplay(float r, float g, float b) = 0;
    virtual void setViewport(double x, double y, double width, double height) = 0;
    virtual void renderTriangleSoup(const std::vector<VertexSimple>& vertices,const glm::vec3& matColor) = 0;
    virtual void setupLight(uint32_t index, glm::vec3 position, glm::vec3 Ia, 
    glm::vec3 Id, glm::vec3 Is) = 0; 
    virtual void turnLight(uint32_t index, bool enable) = 0;
    virtual void setWorldMatrix(const glm::mat4& matrix) = 0;
    virtual const glm::mat4& getWorldMatrix() const = 0;
    virtual void setViewMatrix(const glm::mat4& matrix) = 0;
    virtual const glm::mat4& getViewMatrix() const = 0;
    virtual void setProjMatrix(const glm::mat4& matrix) = 0;
    virtual const glm::mat4& getProjMatrix() const = 0;
    virtual void setRenderMode(RenderMode mode) = 0;
    virtual const RenderMode getRenderMode() const = 0;
};