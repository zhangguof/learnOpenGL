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

using namespace std;

class Shader{
public:
    GLuint Program;
    Shader(const GLchar *vertextSrcPath, const GLchar *fragmentSrcPath);
    void Use();
};


#endif /* shader_hpp */
