//
// Created by marcel on 17.02.21.
//

#ifndef CG_PROJEKT_SKYBOX_H
#define CG_PROJEKT_SKYBOX_H

#include "GL/glew.h"
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "ShaderProgram.h"

#include <vector>
#include <string>

class Skybox {
public:
    Skybox(Camera &camera);
    ~Skybox();
    void init(const char *vertShaderPath, const char *fragShaderPath, std::vector<std::string> texturePaths);
    void uninit();
    void render();
private:
    void createVAO();
    void bindVAO() const;
    void createBuffers();
    void bindBuffers() const;
    void loadBufferData();
    void loadTexture(std::vector<std::string> texturePaths);
    void getShaderPositions();
    void setAttribPointers();
    void bindUniforms();

    GLuint ui_vaoID;
    GLuint ui_vertexListID;
    GLuint ui_indexListID;
    GLuint ui_textureID;
    GLuint ui_samplerID;

    GLint i_vertexAttribPositon;
    GLint i_modelMatrixPosition;
    GLint i_projMatrixPosition;
    GLint i_viewMatrixPosition;
    GLint i_samplerPosition;
    GLint i_eyePosition;
    // vec with vecs of textures
    std::vector<std::vector<unsigned char>> vvuc_textures;
    std::vector<std::string> texturePaths;
    ShaderProgram shaderProgram;

    glm::mat4 mat4_model;
    Camera& camera;

    static const GLfloat af_cubeVertices[];
    static const GLushort as_cubeIndices[];
};


#endif //CG_PROJEKT_SKYBOX_H
