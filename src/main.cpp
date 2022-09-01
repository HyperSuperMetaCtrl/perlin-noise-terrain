//
// Created by marcel on 04.02.21.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#include "Terrain.h"
#include "Camera.h"
#include "Skybox.h"

void error_callback(int error, const char *description) {
    std::cerr << "Error " << description << std::endl;
}

static void keyboard_callback(GLFWwindow *window, int key, int scancode,
                              int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        auto cam = (Camera*) glfwGetWindowUserPointer(window);
        cam->translate(glm::vec3(-1,0,0));
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        auto cam = (Camera*) glfwGetWindowUserPointer(window);
        cam->translate(glm::vec3(1,0,0));
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        auto cam = (Camera*) glfwGetWindowUserPointer(window);
        cam->translate(glm::vec3(0,0,1));
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        auto cam = (Camera*) glfwGetWindowUserPointer(window);
        cam->translate(glm::vec3(0,0,-1));
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
        auto cam = (Camera*) glfwGetWindowUserPointer(window);
        cam->rotate(glm::vec3(1,0,0));
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        auto cam = (Camera *) glfwGetWindowUserPointer(window);
        cam->rotate(glm::vec3(-1, 0, 0));
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        auto cam = (Camera *) glfwGetWindowUserPointer(window);
        cam->rotate(glm::vec3(0, 0, 1));
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        auto cam = (Camera *) glfwGetWindowUserPointer(window);
        cam->rotate(glm::vec3(0, 0, -1));
    }
}
static void resize_callback(GLFWwindow *window, int width, int height) {
   auto *cam = (Camera*)(glfwGetWindowUserPointer(window));
   glViewport(0,0,width,height);
   cam->setAspectRatio((GLfloat) width/(GLfloat) height);
}
int main(int argc, char* argv[]) {
    unsigned int dimension=1000;
    unsigned int heightOct=8, heightFrq=2;
    float heightLucan= 2.5f;
    unsigned int colorOct=8, colorFrq=8;
    float colorLucan = 3.0f;
    std::string textPath = "../resc/text2.png";
    if (argc == 9) {
        dimension = std::stoi(argv[1]);
        heightOct = std::stoi(argv[2]);
        heightFrq = std::stoi(argv[3]);
        heightLucan = std::stof(argv[4]);
        colorOct = std::stoi(argv[5]);
        colorFrq = std::stoi(argv[6]);
        colorLucan = std::stof(argv[7]);
        textPath = argv[8];
    }


    int windowHeight = 600;
    int windowWidth = 800;
    GLFWwindow *window;
    //Camera cam
    Camera cam(glm::vec3(15.0f,30.0f,15.0f),
                                  glm::vec3(10.0f,0.0f,0.0f),
                                  glm::vec3(0.0f,1.0f,0.0f),
                                  glm::radians(45.0f),
                        (GLfloat) windowWidth/(GLfloat) windowHeight,
                                  20.0f,
                                  100.0f);
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "glfwinit failed" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    window = glfwCreateWindow(windowWidth, windowHeight, "Perlin", NULL, NULL);
    if (!window)
        std::cerr << "window creation failed" << std::endl;
    glfwMakeContextCurrent(window);

    glewInit();
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetWindowUserPointer(window, &cam);
    std::vector<std::string> texPaths = {"../resc/sky/right.bmp",
                                         "../resc/sky/left.bmp",
                                         "../resc/sky/top.bmp",
                                         "../resc/sky/bottom.bmp",
                                         "../resc/sky/front.bmp",
                                         "../resc/sky/back.bmp"
                                         };
    Terrain terrain(dimension, dimension,cam);
    terrain.init("../shaders/shader.vert", "../shaders/shader.frag",
                 textPath.c_str(), heightOct,heightFrq,heightLucan,colorOct,colorFrq,colorLucan);
    Skybox skybox(cam);
    skybox.init("../shaders/sky.vert", "../shaders/sky.frag", texPaths);
    static const float black[] = {0.0f,0.0f,0.0f,0.0f};
    glViewport(0,0,windowWidth,windowHeight);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    glClearDepth(300.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        terrain.render();
        skybox.render();
        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    terrain.uninit();
    skybox.uninit();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
