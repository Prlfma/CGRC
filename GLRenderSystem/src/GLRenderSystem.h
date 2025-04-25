#pragma once

#include "../../Interfaces/IRenderSystem.h"
#include "../../ThirdParty\glm/glm.hpp"
#include "../../ThirdParty\glfw\include\GLFW\glfw3.h"
#include <vector>

class GLRenderSystem : public IRenderSystem
{
public:
    void init() override;
    ~GLRenderSystem() = default;
    void clearDisplay(float r, float g, float b) override;
    void setViewport(double x, double y, double width, double height) override;
    void renderTriangleSoup(const std::vector<VertexSimple>& vertices,const glm::vec3& matColor) override;
    void setupLight(uint32_t index, glm::vec3 position, glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is) override;
    void turnLight(uint32_t index, bool enable) override;

    void setWorldMatrix(const glm::mat4& matrix) override;
    const glm::mat4& getWorldMatrix() const override;

    void setViewMatrix(const glm::mat4& matrix) override;
    const glm::mat4& getViewMatrix() const override;

    void setProjMatrix(const glm::mat4& matrix) override;
    const glm::mat4& getProjMatrix() const override;

    void setRenderMode(RenderMode mode) override;  
    const RenderMode getRenderMode() const override;

private:
    // glm::mat4 worldMatrix = glm::mat4(1.0f);
    // glm::mat4 viewMatrix = glm::mat4(1.0f);
    // glm::mat4 projMatrix = glm::mat4(1.0f);
    RenderMode renderMode = RenderMode::Triangles;

   
};