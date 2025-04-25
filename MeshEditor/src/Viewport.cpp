#include "Viewport.h"
#include "../ThirdParty/glm/gtc/matrix_transform.hpp"
#include "../ThirdParty/glm/gtc/type_ptr.hpp"

glm::mat4 Viewport::calcProjectionMatrix() const {
    if (parallelProjection) {
        double w = calcTargetPlaneWidth() / 2.0;
        double h = calcTargetPlaneHeight() / 2.0;
        return glm::ortho(-w, w, -h, h, zNear, zFar);
    } else {
        return glm::perspective(glm::radians(FOV), calcAspectRatio(), zNear, zFar);
    }
}

void Viewport::setViewportSize(uint32_t inWidth, uint32_t inHeight) {
    width = static_cast<double>(inWidth);
    height = static_cast<double>(inHeight);
}

void Viewport::setFOV(double inFOV) { FOV = inFOV; }
void Viewport::setZNear(double inZNear) { zNear = inZNear; }
void Viewport::setZFar(double inZFar) { zFar = inZFar; }
void Viewport::setParallelProjection(bool use) { parallelProjection = use; }

double Viewport::getZNear() const { return zNear; }
double Viewport::getZFar() const { return zFar; }
double Viewport::getFov() const { return FOV; }
double Viewport::getWidth() const { return width; }
double Viewport::getHeight() const { return height; }
bool Viewport::isParallelProjection() const { return parallelProjection; }

double Viewport::calcAspectRatio() const {
    return width / height;
}

double Viewport::calcTargetPlaneHeight() const {
    return 2.0 * camera.distanceFromEyeToTarget() * tan(glm::radians(FOV / 2.0));
}

double Viewport::calcTargetPlaneWidth() const {
    return calcTargetPlaneHeight() * calcAspectRatio();
}

ray Viewport::calcCursorRay(double x, double y) const {
    glm::mat4 view = camera.calcViewMatrix();
    glm::mat4 proj = calcProjectionMatrix();
    glm::vec4 viewport(0.0f, 0.0f, width, height);

    double yInv = height - y;

    glm::vec3 a = glm::unProject(glm::vec3(x, yInv, 0.0), view, proj, viewport);
    glm::vec3 b = glm::unProject(glm::vec3(x, yInv, 1.0), view, proj, viewport);

    return ray{ a, glm::normalize(b - a) };
}

Camera& Viewport::getCamera() {
    return camera;
}

const Camera& Viewport::getCamera() const {
    return camera;
}
