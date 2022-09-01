//
// Created by marcel on 04.02.21.
//

#ifndef CG_PROJEKT_TERRAIN_H
#define CG_PROJEKT_TERRAIN_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <noise/noise.h>

#include <vector>

#include "ShaderProgram.h"
#include "Camera.h"

class Terrain {
public:
    Terrain(unsigned int x, unsigned int z, Camera &cam);

    void init(const char* vertShaderPath,
               const char* fragShaderPath,
               const char* texturePath,
               int heightOct, unsigned int heightFrq, float heightLucan,
               int colorOct, unsigned int colorFrq, float colorLucan);
    void uninit();
    void output_vertices();
    void output_texCoords();
    void output_indexList();

    void render();


private:
    // Number of vertices in x and y direction
    unsigned int ui_XDim;
    unsigned int ui_ZDim;

    std::vector<GLfloat> vf_vertexList;
    std::vector<GLfloat> vf_normalList;
    std::vector<GLuint> vui_indexList;
    std::vector<unsigned char> texture;
    std::vector<GLfloat> vf_texCoords;
    unsigned int txWidth, txHeight;
    // ids for the vao and buffers
    GLuint ui_vaoID;
    GLuint ui_vertexListID;
    GLuint ui_indexListID;
    GLuint ui_textureID;
    GLuint ui_textureCoordsID;
    GLuint ui_samplerID;
    GLuint ui_normalListID;

    ShaderProgram shaderProgram;
    Camera& camera;
    GLint i_vertexAttribPosition;
    GLint i_projMatrixPosition;
    GLint i_viewMatrixPosition;
    GLint i_modelMatrixPosition;
    GLint i_texCoordsPosition;
    GLint i_samplerPosition;
    GLint i_normalsPosition;
    GLint i_nModelPosition;

    noise::module::Perlin heightNoise;
    noise::module::Perlin colorNoise;

    glm::mat4 mat4_model;
    glm::mat3 mat3_nmodel;

    void createVAO();
    void bindVAO();
    void createBuffers();
    void bindBuffers() const;
    void loadBufferData();
    void bindUniforms();
    void loadTexture(const char * texturePath);
    void initPerlinNoise(int heightOct, unsigned int heightFrq, float heightLucan,
                         int colorOct, unsigned int colorFrq, float colorLucan);
    // generate vertex data
    void generateTexCoords();
    void generateVertexList();
    void generateIndexList();
    void generateNormals();
    void getShaderPositions();
    void setAttribPointers();
};

#endif // CG_PROJEKT_TERRAIN_H
