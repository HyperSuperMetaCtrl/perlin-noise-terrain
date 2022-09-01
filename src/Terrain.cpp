//
// Created by marcel on 04.02.21.
//

#include "Terrain.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glm/gtx/string_cast.hpp>
#include "lodepng.h"
Terrain::Terrain(const unsigned int x, const unsigned int z,
                 Camera& cam) :camera(cam)
                 {

    ui_XDim = x;
    ui_ZDim = z;
    ui_vaoID = 0;
    ui_vertexListID = 0;
    ui_indexListID = 0;
    ui_textureID = 0;
    ui_textureCoordsID = 0;
    ui_samplerID = 0;
    ui_normalListID = 0;
    i_vertexAttribPosition = 0;
    i_projMatrixPosition = 0;
    i_modelMatrixPosition = 0;
    i_texCoordsPosition = 0;
    i_nModelPosition = 0;
    mat4_model = glm::scale(glm::mat4(1.0f), glm::vec3(35.0f, 5.0f, 35.0f));

}

void Terrain::init(const char* vertShaderPath,
                   const char* fragShaderPath,
                   const char* texturePath,
                   const int heightOct, const unsigned int heightFrq, float heightLucan,
                   const int colorOct, const unsigned int colorFrq, float colorLucan
                   ) {
    glm::mat4 tmp =  glm::transpose(glm::inverse(mat4_model));
    mat3_nmodel = glm::mat3(tmp[0],tmp[1],tmp[2]);
    createVAO();
    bindVAO();
    shaderProgram.addShader(vertShaderPath, GL_VERTEX_SHADER);
    shaderProgram.addShader(fragShaderPath, GL_FRAGMENT_SHADER);
    shaderProgram.linkShaders();
    getShaderPositions();
    initPerlinNoise(heightOct, heightFrq, heightLucan,
                    colorOct, colorFrq, colorLucan);
    generateVertexList();
    generateIndexList();
    generateNormals();
    generateTexCoords();
    createBuffers();
    bindBuffers();
    loadTexture(texturePath);
    loadBufferData();
    setAttribPointers();
    glBindVertexArray(0);
}

void Terrain::generateVertexList() {
    GLfloat dx = 2.0f / ((GLfloat) ui_XDim - 1.0f);
    GLfloat dz = 2.0f / ((GLfloat) ui_ZDim - 1.0f);

    GLfloat x_coord;
    GLfloat z_coord;
    GLfloat y_coord;
    // generate grid row first
    for (unsigned int row = 0; row < ui_ZDim; row++) {
        for (unsigned int col = 0; col < ui_XDim; col++) {
            x_coord =  (col * dx) - 1;
            z_coord =  (row * dz) - 1;
            y_coord = glm::pow((heightNoise.GetValue(x_coord, 1, z_coord)*0.5f+0.5f), 2.0f);
            vf_vertexList.push_back(x_coord);
            vf_vertexList.push_back(y_coord);
            vf_vertexList.push_back(z_coord);
        }
    }
}

void Terrain::output_vertices() {
    for (unsigned int i = 0; i < vf_vertexList.size(); i++) {
        std::cout << vf_vertexList[i] << ",";
        if (i % 3 == 2)
            std::cout << std::endl;
    }
}
void Terrain::generateIndexList() {
    for (unsigned int row = 0; row < ui_ZDim - 1; row++) {
        for (unsigned int col = 0; col < ui_XDim - 1; col++) {
            vui_indexList.push_back(col + (row * ui_XDim));
            vui_indexList.push_back(col + (row * ui_XDim) + 1);
            vui_indexList.push_back(col + (row * ui_XDim) + ui_XDim);
            vui_indexList.push_back(col + (row * ui_XDim) + 1);
            vui_indexList.push_back(col + (row * ui_XDim) + ui_XDim + 1);
            vui_indexList.push_back(col + (row * ui_XDim) + ui_XDim);
        }
    }
}

void Terrain::generateNormals() {
    int i1, i2, i3;
    glm::vec3 x1, x2, x3;
    glm::vec3 t1, t2;
    glm::vec3 n;
    vf_normalList = std::vector<GLfloat>(vf_vertexList.size(), 0.0);
    for (unsigned int i = 0; i < vui_indexList.size(); i += 3) {
        //get indices of triangle corners
        i1 = 3*vui_indexList[i];
        i2 = 3*vui_indexList[i + 1];
        i3 = 3*vui_indexList[i + 2];
        //get vectors
        x1 = glm::vec3(vf_vertexList[i1],
                       vf_vertexList[i1 + 1],
                       vf_vertexList[i1 + 2]);
        x2 = glm::vec3(vf_vertexList[i2],
                       vf_vertexList[i2 + 1],
                       vf_vertexList[i2 + 2]);
        x3 = glm::vec3(vf_vertexList[i3],
                       vf_vertexList[i3 + 1],
                       vf_vertexList[i3 + 2]);
        // build tagential vecs
        t1 = x2 - x1;
        t2 = x3 - x1;
        n = glm::normalize(glm::cross(t2, t1));
        vf_normalList[i1] += n.x;
        vf_normalList[i1+1] += n.y;
        vf_normalList[i1+2] += n.z;
        vf_normalList[i2] += n.x;
        vf_normalList[i2+1] += n.y;
        vf_normalList[i2+2] += n.z;
        vf_normalList[i3] += n.x;
        vf_normalList[i3+1] += n.y;
        vf_normalList[i3+2] += n.z;
    }
}

void Terrain::initPerlinNoise(int heightOct, unsigned int heightFrq, float heightLucan,
                              int colorOct, unsigned int colorFrq, float colorLucan) {
    std::srand(std::time(nullptr));
    int randint = std::rand();
    heightNoise.SetSeed(randint);
    heightNoise.SetOctaveCount(heightOct);
    heightNoise.SetFrequency(heightFrq);
    heightNoise.SetLacunarity(heightLucan);
    randint = std::rand();
    colorNoise.SetSeed(randint);
    colorNoise.SetOctaveCount(colorOct);
    colorNoise.SetFrequency(colorFrq);
    colorNoise.SetLacunarity(colorLucan);
}

void Terrain::createVAO() {
    glCreateVertexArrays(1, &ui_vaoID);
}

void Terrain::bindVAO() {
    glBindVertexArray(ui_vaoID);
}

void Terrain::createBuffers() {
    glCreateBuffers(1, &ui_vertexListID);
    glCreateBuffers(1, &ui_indexListID);
    glCreateBuffers(1, &ui_textureCoordsID);
    glCreateBuffers(1, &ui_normalListID);
}

void Terrain::bindBuffers() const {
    glBindBuffer(GL_ARRAY_BUFFER, ui_vertexListID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_indexListID);
    glBindBuffer(GL_ARRAY_BUFFER, ui_textureCoordsID);
    glBindBuffer(GL_ARRAY_BUFFER, ui_normalListID);
}

void Terrain::output_indexList() {
    for (float i : vui_indexList) {
        std::cout << i << ",";
    }
}

void Terrain::loadBufferData() {
    glNamedBufferStorage(ui_vertexListID,
                         vf_vertexList.size() * sizeof(GLfloat),
                         &vf_vertexList[0],
                         GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ui_indexListID,
                         vui_indexList.size() * sizeof(GLuint),
                         &vui_indexList[0],
                         GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ui_textureCoordsID,
                         vf_texCoords.size() * sizeof(GLfloat),
                         &vf_texCoords[0],
                         GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ui_normalListID,
                         vf_normalList.size() * sizeof(GLfloat),
                         &vf_normalList[0],
                         GL_DYNAMIC_STORAGE_BIT);
}
void Terrain::getShaderPositions(){
    i_vertexAttribPosition = glGetAttribLocation(shaderProgram.getProgID(), "in_position");
    i_texCoordsPosition = glGetAttribLocation(shaderProgram.getProgID(), "in_texCoords");
    i_normalsPosition = glGetAttribLocation(shaderProgram.getProgID(), "in_normals");
    i_projMatrixPosition = glGetUniformLocation(shaderProgram.getProgID(), "proj");
    i_viewMatrixPosition = glGetUniformLocation(shaderProgram.getProgID(), "view");
    i_modelMatrixPosition = glGetUniformLocation(shaderProgram.getProgID(), "model");
    i_samplerPosition = glGetUniformLocation(shaderProgram.getProgID(), "myTex");
    i_nModelPosition = glGetUniformLocation(shaderProgram.getProgID(), "nmodel");
}
void Terrain::setAttribPointers() {
    glBindBuffer(GL_ARRAY_BUFFER, ui_vertexListID);
    glVertexAttribPointer(i_vertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(i_vertexAttribPosition);
    glBindBuffer(GL_ARRAY_BUFFER, ui_textureCoordsID);
    glVertexAttribPointer(i_texCoordsPosition, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(i_texCoordsPosition);
    glBindBuffer(GL_ARRAY_BUFFER, ui_normalListID);
    glVertexAttribPointer(i_normalsPosition, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(i_normalsPosition);
}
void Terrain::bindUniforms() {
    //bind uniform matrices
    glUniformMatrix4fv(i_projMatrixPosition, 1, GL_FALSE, glm::value_ptr(camera.getProj()));
    glUniformMatrix4fv(i_viewMatrixPosition, 1, GL_FALSE, glm::value_ptr(camera.getView()));
    glUniformMatrix4fv(i_modelMatrixPosition, 1, GL_FALSE, glm::value_ptr(this->mat4_model));
    glUniformMatrix3fv(i_nModelPosition, 1, GL_FALSE, glm::value_ptr(mat3_nmodel));
    glUniform1i(i_samplerPosition, 0);
}

void Terrain::render() {
    bindVAO();
    shaderProgram.useProgram();
    bindUniforms();
    glDrawElements(GL_TRIANGLES, vui_indexList.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Terrain::loadTexture(const char * texturePath) {
    unsigned int error = lodepng::decode(texture, txWidth, txHeight, texturePath);
    if (error) {
        std::cerr << "error loading texture" << error << std::endl;
    }
    glCreateTextures(GL_TEXTURE_2D, 1, &ui_textureID);
    glBindTextureUnit(0, ui_textureID);
    glTextureStorage2D(ui_textureID, 1, GL_SRGB8, txWidth, txHeight);
    glTextureSubImage2D(ui_textureID,
                        0,
                        0, 0,
                        txWidth, txHeight,
                        GL_RGBA, GL_UNSIGNED_BYTE,
                        &texture[0]);
    glCreateSamplers(1, &ui_samplerID);
    glBindSampler(0, ui_samplerID);
    //set parameters
    glTextureParameteri(ui_textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(ui_samplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ui_textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(ui_samplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(ui_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(ui_samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(ui_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(ui_samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture.clear();
}

void Terrain::generateTexCoords() {
    for (unsigned int i = 0; i < vf_vertexList.size(); i += 3) {
        GLfloat v = vf_vertexList[i + 1];
        GLfloat u = colorNoise.GetValue(vf_vertexList[i],
                                        vf_vertexList[i + 1],
                                        vf_vertexList[i + 2]);
        //u = (1+u)*0.5;
        vf_texCoords.push_back(1 - u);
        vf_texCoords.push_back(1 - v);
    }
}

void Terrain::uninit() {
    glDeleteBuffers(1, &ui_vertexListID);
    glDeleteBuffers(1, &ui_indexListID);
    glDeleteTextures(1, &ui_textureID);
    glDeleteBuffers(1, &ui_textureCoordsID);
    glDeleteSamplers(1, &ui_samplerID);
    glDeleteBuffers(1, &ui_normalListID);
    glDeleteVertexArrays(1, &ui_vaoID);
    vf_normalList.clear();
    vf_vertexList.clear();
    vf_texCoords.clear();
    vui_indexList.clear();
    texture.clear();
}
