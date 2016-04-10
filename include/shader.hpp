//
//  shader.hpp
//  shadertest
//
//  Created by tony on 16/3/21.
//  Copyright © 2016年 tony. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <stdio.h>
#include <map>

using namespace std;

class Shader{
public:
    GLuint Program;
    map<string,GLuint> uniform_locals;
    
    GLint get_unifrom_loc(const char* var_name);
    void setUniform3f(const char* var_name,GLfloat x,GLfloat y, GLfloat z);
    //glUniformMatrix4fv
    
    void setUniformMatrix4fv(const char* var_name,const GLfloat *mat4);
    void setUniform1f(const char* var_name,GLfloat x);
    Shader(const GLchar *vertextSrcPath, const GLchar *fragmentSrcPath);
    void Use();
};


#endif /* shader_hpp */
