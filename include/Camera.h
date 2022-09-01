//
// Created by marcel on 13.02.21.
// inspired by learnopengls camera guide
//

#ifndef CG_PROJEKT_CAMERA_H
#define CG_PROJEKT_CAMERA_H
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera {
public:
    Camera(glm::vec3 loc,
           glm::vec3 lookAt,
           glm::vec3 up,
           GLfloat fov,
           GLfloat aspectRatio,
           GLfloat near,
           GLfloat far);
    ~Camera();

    void setView(glm::mat4 view);
    void setProj(glm::mat4 proj);

    void setAspectRatio(GLfloat aspectRatio);
    void setNear(GLfloat near);
    void setFar(GLfloat far);
    void setFov(GLfloat fov);
    void setLookAt(glm::vec3 lookAt);
    void setUp(glm::vec3 up);
    void setLoc(glm::vec3 loc);

    GLfloat getAspectRatio();
    GLfloat getNear();
    GLfloat getFar();
    GLfloat getFov();
    glm::vec3 getLocation();
    glm::vec3 getLookAt();
    glm::vec3 getUp();

    glm::mat4 getView();
    glm::mat4 getProj();
    glm::mat4 getViewProj();

    void translate(glm::vec3 direction);
    void rotate(glm::vec3 direction);

private:
    GLfloat f_fov;
    GLfloat f_aspectRatio;
    GLfloat f_near;
    GLfloat f_far;

    glm::vec3 v3f_location;
    glm::vec3 v3f_lookAt;
    glm::vec3 v3f_up;

    glm::mat4 mat4_view;
    glm::mat4 mat4_proj;

    void updateCamera();
};


#endif //CG_PROJEKT_CAMERA_H
