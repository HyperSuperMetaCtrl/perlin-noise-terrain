//
// Created by marcel on 10.02.21.
// mixture of the GLSample Shaderprogram and learnOpenGL Shader class
//

#ifndef CG_PROJEKT_SHADERPROGRAM_H
#define CG_PROJEKT_SHADERPROGRAM_H
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <GL/gl.h>

class ShaderProgram {
public:
    ShaderProgram();

    ~ShaderProgram();

    void addShader(const char *filepath, GLenum shaderType);

    void linkShaders();

    void useProgram();
    GLint getProgID();
private:
    GLint ui_shaderprogram;
    std::vector<GLuint> vui_shaders;

    bool checkErrors(GLuint id, std::string type);
};


#endif //CG_PROJEKT_SHADERPROGRAM_H
