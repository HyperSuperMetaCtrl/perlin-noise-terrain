//
// Created by marcel on 10.02.21.
//

#include <sstream>
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
    ui_shaderprogram = glCreateProgram();
    vui_shaders.reserve(0);
}
ShaderProgram::~ShaderProgram()
{
    // free all shader objects
    for( auto i=vui_shaders.begin(); i != vui_shaders.end(); ++i) {
        glDeleteShader( *i );
    }

    // free program
    if( ui_shaderprogram) glDeleteProgram( ui_shaderprogram );
}

void ShaderProgram::addShader(const char *filepath, GLenum shaderType) {
    std::string shaderCode;
    std::ifstream shaderFile;
    std::string str_shaderType = (shaderType == GL_VERTEX_SHADER
            ? "VERTEX" : "FRAGMENT");
    shaderFile.exceptions(std::ifstream::failbit|std::fstream::badbit);
    try {
        shaderFile.open(filepath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure& e){
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* cstr_ShaderCode = shaderCode.c_str();
    GLint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &cstr_ShaderCode, NULL);
    glCompileShader(shaderID);
    if (checkErrors(shaderID, str_shaderType)) {
        vui_shaders.push_back(shaderID);
        glAttachShader(ui_shaderprogram,shaderID);
    }
}

bool ShaderProgram::checkErrors(GLuint id, std::string type) {
    GLint success;
    GLchar* Infolog;
    GLint iLength;
    if (type != "PROGRAM") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &iLength);
            Infolog = new char[iLength];
            glGetShaderInfoLog(id,iLength,&iLength, Infolog);
            std::cerr << "Compile Error for " << type << " Shader " <<  Infolog << std::endl;
            delete[] Infolog;
            return false;
        }
    }
    else {
        glGetProgramiv(ui_shaderprogram,GL_LINK_STATUS,&success);
        if (!success) {
            glGetProgramiv(ui_shaderprogram,GL_INFO_LOG_LENGTH, &iLength);
            Infolog = new char[iLength];
            glGetProgramInfoLog(id,iLength,&iLength,Infolog);
            std::cerr << "Link Error:" << Infolog << std::endl;
            delete[] Infolog;
            return false;
        }
    }
    return true;
}

void ShaderProgram::linkShaders() {
    glLinkProgram(ui_shaderprogram);
    checkErrors(ui_shaderprogram, "PROGRAM");
    useProgram();
}

void ShaderProgram::useProgram() {
    glUseProgram(ui_shaderprogram);
}

GLint ShaderProgram::getProgID() {
    return this->ui_shaderprogram;
}
