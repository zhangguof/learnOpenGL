//
//  gltools.cpp
//  gltools
//
//  Created by tony on 16/4/2.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "gltools.hpp"
#include <GLFW/glfw3.h>
#include "Model.hpp"

GLFWwindow* create_window(int width, int height,
                   const char *titile, GLFWmonitor *monitor, GLFWwindow* share){
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }
// using opengGL3.3+
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(width, height, titile, monitor, share);
    if(!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    return window;
}

//vao vbo
void VBO::bindVBO(){
    size_t size = sizeof(GLfloat) * num * strip;
    GLint v_att_offest = 0;
    GLint uniform_loc = 0;

    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,size,vertices,draw_usage);
    glVertexAttribPointer(uniform_loc, 3, GL_FLOAT, GL_FALSE,
                          strip*sizeof(GL_FLOAT),(GLvoid *)0);
    glEnableVertexAttribArray(uniform_loc);
    v_att_offest += 3;
    uniform_loc++;
    
    if(enableCorlor){
        glVertexAttribPointer(uniform_loc,3,GL_FLOAT, GL_FALSE,
                              strip*sizeof(GL_FLOAT),
                              (GLvoid *)(v_att_offest * sizeof(GLfloat)));
        glEnableVertexAttribArray(uniform_loc);
        v_att_offest += 3;
        uniform_loc++;
    }
    if(enableTexcoord){
        glVertexAttribPointer(uniform_loc,2,GL_FLOAT, GL_FALSE,
                              strip*sizeof(GL_FLOAT),
                              (GLvoid*)(v_att_offest * sizeof(GLfloat)));
        glEnableVertexAttribArray(uniform_loc);
        v_att_offest += 2;
        uniform_loc++;
    }
    if(enableNormal){
        
        glVertexAttribPointer(uniform_loc,3,GL_FLOAT,GL_FALSE,
                              strip*sizeof(GL_FLOAT),
                              (GLvoid *)(v_att_offest * sizeof(GLfloat)));
        glEnableVertexAttribArray(uniform_loc);
        //printf("Nor_loc:%d,v_att_offest:%d\n",NOR_LOC,v_att_offest);
        printf("uniform_loc:%d\n",uniform_loc);
        v_att_offest += 3;
        uniform_loc++;
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
}
