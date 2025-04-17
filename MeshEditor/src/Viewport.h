#pragma once
#include "Camera.h"
#include "../ThirdParty/glm/glm.hpp"

struct ray {
    glm::vec3 orig;
    glm::vec3 dir{ 0,0,1 };
};

class Viewport {
public:
    glm::mat4 calcProjectionMatrix() const;

    void setViewportSize(uint32_t inWidth, uint32_t inHeight);
    void setFOV(double inFOV);
    void setZNear(double inZNear);
    void setZFar(double inZFar);
    void setParallelProjection(bool use);

    double getZNear() const;
    double getZFar() const;
    double getFov() const;
    double getWidth() const;
    double getHeight() const;
    bool isParallelProjection() const;

    ray calcCursorRay(double x, double y) const;
    double calcTargetPlaneWidth() const;
    double calcTargetPlaneHeight() const;
    double calcAspectRatio() const;

    Camera& getCamera();
    const Camera& getCamera() const;

private:
    double FOV = 60.0;
    double zNear = 0.01;
    double zFar = 500.0;
    double width = 1.0;
    double height = 1.0;
    bool parallelProjection = false;
    Camera camera;
};
