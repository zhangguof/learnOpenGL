//
//  shader.cpp
//  shadertest
//
//  Created by tony on 16/3/21.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "shader.hpp"

Shader::Shader(const GLchar *vertexSrcPath, const GLchar *fragmentSrcPaht){
    string verCode, fragCode;
    ifstream vShaderFile, fShaderFile;
    
    vShaderFile.exceptions(ifstream::failbit);
    fShaderFile.exceptions(ifstream::failbit);

    try {
        vShaderFile.open(vertexSrcPath);
        fShaderFile.open(fragmentSrcPaht);
        stringstream verstream, fragstream;
        verstream << vShaderFile.rdbuf();
        fragstream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        verCode = verstream.str();
        fragCode = fragstream.str();
        
    } catch (ifstream::failure e) {
        cout<<"ERROR:SHADER::FILE_READ_FAILE.\n"<<e.what()<<endl;
        return;
    }
    const GLchar* vShaderCode = verCode.c_str();
    const GLchar* fShaderCode = fragCode.c_str();
    GLuint vertex, fragment;
    GLint success;
    GLchar infolog[512];
    
    //vertext shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infolog<<endl;
    }
    
    //fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infolog);
        cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infolog<<endl;
    }
    
    //shader program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->Program, 512, NULL, infolog);
        cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n"<<infolog<<endl;
        
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    
}

void Shader::Use(){
    glUseProgram(this->Program);
    
}

GLint Shader::get_unifrom_loc(const char* var_name)
{
    GLint cur_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);
    if(cur_program != Program)
    {
        cout<<"ERROR:CURRENT PROGRAM NOT ACTIVE!"<<endl;
        return -2;
    }
    
    auto loc_iter = uniform_locals.find(var_name);
    GLint loc;
    if(loc_iter != uniform_locals.end())
    {
        loc = loc_iter->second;
        return loc;
    }
    else
    {
        loc = glGetUniformLocation(Program, var_name);
        if (loc==-1)
        {
            cout<<"ERROR:GETUNIFORM:"<<var_name<<endl;
            return loc;
        }
    }
    uniform_locals[var_name] = loc;
    return loc;
}


void Shader::setUniform3f(const char *var_name, GLfloat x, GLfloat y, GLfloat z)
{
    GLint loc = get_unifrom_loc(var_name);
    if(loc < 0)
        return;
    glUniform3f(loc,x,y,z);
    
}
void Shader::setUniform1f(const char* var_name,GLfloat x)
{
    GLint loc = get_unifrom_loc(var_name);
    if(loc >= 0)
        glUniform1f(loc,x);
    
}
void Shader::setUniformMatrix4fv(const char* var_name, const GLfloat * mat4)
{
    GLint loc = get_unifrom_loc(var_name);
    if(loc >= 0)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, mat4);
    }
    
}

