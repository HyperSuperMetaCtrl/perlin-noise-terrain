//
// Created by marcel on 17.02.21.
//

#include "Skybox.h"

#include <iostream>
#include "stb_image.h"

// vertices and indices from redbook examaple06-cubemap
const GLfloat Skybox::af_cubeVertices[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f
};
const GLushort Skybox::as_cubeIndices[] = {
        0, 1, 2, 3, 6, 7, 4, 5,         // First strip
        2, 6, 0, 4, 1, 5, 3, 7          // Second strip
};

Skybox::Skybox(Camera &cam): camera(cam) {
    mat4_model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 30.0f, 50.0f));
}
void Skybox::init(const char * vertShaderPath, const char * fragShaderPath, std::vector<std::string> texturePaths) {
    createVAO();
    bindVAO();
    shaderProgram.addShader(vertShaderPath, GL_VERTEX_SHADER);
    shaderProgram.addShader(fragShaderPath, GL_FRAGMENT_SHADER);
    shaderProgram.linkShaders();
    getShaderPositions();
    createBuffers();
    bindBuffers();
    loadBufferData();
    loadTexture(texturePaths);
    setAttribPointers();
    glBindVertexArray(0);
}
Skybox::~Skybox() =default;


void Skybox::render() {
    bindVAO();
    shaderProgram.useProgram();
    bindUniforms();
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, nullptr);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (void *) (8 * sizeof(GLushort)));

}

void Skybox::createVAO() {
    glCreateVertexArrays(1, &ui_vaoID);
}

void Skybox::bindVAO() const {
    glBindVertexArray(ui_vaoID);
}

void Skybox::createBuffers() {
    glCreateBuffers(1, &ui_vertexListID);
    glCreateBuffers(1, &ui_indexListID);

}

void Skybox::bindBuffers() const {
    glBindBuffer(GL_ARRAY_BUFFER, ui_vertexListID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_indexListID);
}

void Skybox::loadBufferData() {
    glNamedBufferStorage(ui_vertexListID,
                         3*8*sizeof(GLfloat),
                         af_cubeVertices,
                         GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ui_indexListID,
                         16*sizeof(GLushort),
                         as_cubeIndices,
                         GL_DYNAMIC_STORAGE_BIT);
}

void Skybox::getShaderPositions() {
    i_vertexAttribPositon = glGetAttribLocation(shaderProgram.getProgID(), "in_position");
    i_modelMatrixPosition = glGetUniformLocation(shaderProgram.getProgID(), "model");
    i_projMatrixPosition = glGetUniformLocation(shaderProgram.getProgID(), "proj");
    i_viewMatrixPosition = glGetUniformLocation(shaderProgram.getProgID(), "view");
    i_samplerPosition = glGetUniformLocation(shaderProgram.getProgID(), "myTex");
    i_eyePosition = glGetUniformLocation(shaderProgram.getProgID(), "eyepos");
}

void Skybox::setAttribPointers() {
    glBindBuffer(GL_ARRAY_BUFFER, ui_vertexListID);
    glVertexAttribPointer(i_vertexAttribPositon, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(i_vertexAttribPositon);

}
void Skybox::bindUniforms() {
    glUniformMatrix4fv(i_modelMatrixPosition, 1, GL_FALSE, glm::value_ptr(this->mat4_model));
    glUniformMatrix4fv(i_projMatrixPosition, 1, GL_FALSE, glm::value_ptr(camera.getProj()));
    glUniformMatrix4fv(i_viewMatrixPosition, 1, GL_FALSE, glm::value_ptr(camera.getView()));
    glUniform1i(i_samplerPosition, 1);
    glUniform3fv(i_eyePosition, 1, glm::value_ptr(camera.getLocation()));
}

void Skybox::loadTexture(std::vector<std::string> filepaths) {
    int numberOfFaces = filepaths.size();
    int txWidth, txHeight, numberOfChannels;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &ui_textureID);
    glBindTextureUnit(1, ui_textureID);
    glTextureStorage2D(ui_textureID, 10, GL_RGBA8, 512,512);
    for (int i = 0; i < numberOfFaces; i++) {
        unsigned char *image = stbi_load(filepaths[i].c_str(), &txWidth, &txHeight, &numberOfChannels, 0);
       glTextureSubImage3D(ui_textureID,
                           0,
                           0,0,
                           i,
                           txWidth, txHeight,
                           1,
                           GL_RGB, GL_UNSIGNED_BYTE,
                           image);
    }
    glCreateSamplers(1, &ui_samplerID);
    glBindSampler(1, ui_samplerID);
    glTextureParameteri(ui_textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(ui_samplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ui_textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(ui_samplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ui_textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(ui_samplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(ui_textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(ui_samplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void Skybox::uninit() {
    glDeleteBuffers(1, &ui_vertexListID);
    glDeleteBuffers(1, &ui_indexListID);
    glDeleteSamplers(1,&ui_samplerID);
    glDeleteTextures(1, &ui_textureID);
    glDeleteVertexArrays(1,&ui_vaoID);
}
