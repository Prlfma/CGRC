#include "Camera.h"
#include "../ThirdParty/glm/gtc/matrix_transform.hpp"
#include "../ThirdParty/glm/gtx/rotate_vector.hpp"
#include <cmath>

glm::mat4 Camera::calcViewMatrix() const {
    return glm::lookAt(eye, target, up);
}

glm::vec3 Camera::calcForward() const {
    return glm::normalize(target - eye);
}

glm::vec3 Camera::calcRight() const {
    return glm::normalize(glm::cross(calcForward(), up));
}

double Camera::distanceFromEyeToTarget() const {
    return glm::distance(eye, target);
}

const glm::vec3& Camera::getEye() const {
    return eye;
}

const glm::vec3& Camera::getTarget() const {
    return target;
}
const glm::vec3& Camera::getUp() const {
    return up;
}
void Camera::setFrontView() {
    eye = target + glm::vec3(0, 0, 10);
    up = glm::vec3(0, 1, 0);
}

void Camera::setTopView() {
    eye = target + glm::vec3(0, 10, 0);
    up = glm::vec3(0, 0, -1);
}

void Camera::setRearView() {
    eye = target + glm::vec3(0, 0, -10);
    up = glm::vec3(0, 1, 0);
}

void Camera::setRightView() {
    eye = target + glm::vec3(10, 0, 0);
    up = glm::vec3(0, 1, 0);
}

void Camera::setLeftView() {
    eye = target + glm::vec3(-10, 0, 0);
    up = glm::vec3(0, 1, 0);
}

void Camera::setBottomView() {
    eye = target + glm::vec3(0, -10, 0);
    up = glm::vec3(0, 0, 1);
}

void Camera::setIsoView() {
    eye = glm::vec3(10.f) + target + glm::normalize(glm::vec3(1, 1, 1));
    up = glm::vec3(0, 1, 0);
}

void Camera::orbit(glm::vec3 a, glm::vec3 b) {
   // Розраховуємо різницю між векторами (аналог руху миші)
   glm::vec2 delta(b.x - a.x, b.y - a.y);
   const float sensitivity = 0.005f;  // Можна зробити параметром функції
   
   // Горизонтальне обертання (навколо up-вектора)
   rotate(target, up, -delta.x * sensitivity);
   
   // Вертикальне обертання (навколо правої осі)
   rotate(target, calcRight(), -delta.y * sensitivity);
}

void Camera::pan(double u, double v) {
    // Обчислюємо вектори напрямку
    glm::vec3 right = calcRight();
    glm::vec3 upVec = up;
    
    // Створюємо вектор зміщення
    glm::vec3 delta = static_cast<float>(u) * right + static_cast<float>(v) * upVec;
    
    // Створюємо матрицю переміщення
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), delta);
    
    // Застосовуємо перетворення
    transform(translation);
}

void Camera::zoom(double factor) {
    glm::vec3 direction = calcForward();
    eye += direction * static_cast<float>(factor);
}

void Camera::translate(glm::vec3 delta) {
    eye += delta;
    target += delta;
}

void Camera::setDistanceToTarget(double D) {
    glm::vec3 direction = glm::normalize(eye - target);
    eye = target + direction * static_cast<float>(D);
}

void Camera::transform(const glm::mat4& trf) {
    glm::vec4 newEye = trf * glm::vec4(eye, 1.0f);
    glm::vec4 newTarget = trf * glm::vec4(target, 1.0f);
    glm::vec4 newUp = trf * glm::vec4(up, 0.0f);  
    eye = glm::vec3(newEye);
    target = glm::vec3(newTarget);
    up = glm::normalize(glm::vec3(newUp));
}

void Camera::rotate(glm::vec3 point, glm::vec3 axis, double angle) {
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)angle, axis);
    glm::vec3 direction = eye - point;
    direction = glm::vec3(rot * glm::vec4(direction, 0.0f));
    eye = point + direction;
    up = glm::normalize(glm::vec3(rot * glm::vec4(up, 0.0f)));
}

void Camera::setEyeTargetUp(glm::vec3 newEye, glm::vec3 newTarget, glm::vec3 newUp) {
    eye = newEye;
    target = newTarget;
    up = glm::normalize(newUp);
}
