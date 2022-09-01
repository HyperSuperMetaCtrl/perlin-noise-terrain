//
// Created by marcel on 13.02.21.
//

#include "Camera.h"

Camera::Camera(glm::vec3 loc, glm::vec3 lookAt, glm::vec3 up, GLfloat fov, GLfloat aspectRatio, GLfloat near,
               GLfloat far) {
    v3f_location = loc;
    v3f_lookAt = lookAt;
    v3f_up = up;
    f_aspectRatio = aspectRatio;
    f_near = near;
    f_far = far;
    f_fov = fov;
    mat4_view = glm::lookAt(loc, lookAt, up);
    mat4_proj = glm::perspective(fov,aspectRatio,near,far);
}
Camera::~Camera() {}
void Camera::updateCamera() {
    this->mat4_view = glm::lookAt(v3f_location, v3f_lookAt, v3f_up);
    this->mat4_proj = glm::perspective(f_fov,f_aspectRatio,f_near,f_far);
}

void Camera::setAspectRatio(GLfloat aspectRatio) {
    this->f_aspectRatio = aspectRatio;
    updateCamera();
}

void Camera::setNear(GLfloat near) {
    this->f_near = near;
    updateCamera();
}

void Camera::setFar(GLfloat far) {
    this->f_far = far;
    updateCamera();
}

void Camera::setFov(GLfloat fov) {
    this->f_fov = fov;
    updateCamera();
}

void Camera::setLookAt(glm::vec3 lookAt) {
    this->v3f_lookAt = lookAt;
    updateCamera();
}

void Camera::setUp(glm::vec3 up) {
    this->v3f_up = up;
    updateCamera();
}

GLfloat Camera::getAspectRatio() {
    return this->f_aspectRatio;
}

GLfloat Camera::getNear() {
    return this->f_near;
}

GLfloat Camera::getFar() {
    return this->f_far;
}

GLfloat Camera::getFov() {
    return this->f_fov;
}

glm::vec3 Camera::getLookAt() {
    return this->v3f_lookAt;
}

glm::vec3 Camera::getUp() {
    return this->v3f_up;
}

glm::mat4 Camera::getView() {
    return this->mat4_view;
}

glm::mat4 Camera::getProj() {
    return this->mat4_proj;
}

glm::mat4 Camera::getViewProj() {
    return this->mat4_proj * this->mat4_view;
}

void Camera::setView(glm::mat4 view) {
    this->mat4_view = view;
}

void Camera::setProj(glm::mat4 proj) {
    this->mat4_proj = proj;
}

void Camera::setLoc(glm::vec3 loc) {
    this->v3f_location = loc;
    updateCamera();
}

void Camera::translate(glm::vec3 direction) {
    v3f_location = v3f_location + direction;
    v3f_lookAt = v3f_lookAt + direction;
    updateCamera();
}

void Camera::rotate(glm::vec3 direction) {
    v3f_lookAt = v3f_lookAt + direction;
    updateCamera();
}

glm::vec3 Camera::getLocation() {
    return this->v3f_location;
}


