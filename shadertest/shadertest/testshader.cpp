//
//  testshader.cpp
//  shadertest
//
//  Created by tony on 16/3/21.
//  Copyright © 2016年 tony. All rights reserved.
//

#include <stdio.h>
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

const int win_width=640,win_height=480;


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


GLuint VAO[2],VBO[2];
Shader *pshader;

void gen_vao_vbo(GLuint *vao, GLuint *vbo, GLfloat vertices[],size_t size)
{
    glGenVertexArrays(1,vao);
    glGenBuffers(1, vbo);
    
    glBindVertexArray(*vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER,size, vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
}
void gen_vao_vbo_color(GLuint *vao, GLuint *vbo, GLfloat vertices[],size_t size)
{
    glGenVertexArrays(1,vao);
    glGenBuffers(1, vbo);
    
    glBindVertexArray(*vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER,size, vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat),
                          (GLvoid *)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
}

void init_render(GLuint *vao, GLuint *vbo)
{
    GLfloat vertices[] = {
        // 第一个三角形
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, 0.5f, 0.0f,  // 左上角
    };
    GLfloat vertices2[]={
        // 第二个三角形       //color
        0.5f, -0.5f, 0.0f,  1.0f,0.0f,0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f, // 左下角
        -0.5f, 0.5f, 0.0f,  0.0f,0.0f,1.0f,   // 左上角
    };

    
    
    gen_vao_vbo(vao, vbo, vertices,sizeof(vertices));
    gen_vao_vbo_color(&vao[1],&vbo[1], vertices2,sizeof(vertices2));
    
}




int main(int argc, char **argv)
{
    
    printf("start opengl\n");
    
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(win_width,win_height, "test3d", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    
    glewExperimental = GL_TRUE;
    glewInit();

    
    init_render(VAO, VBO);
    pshader = new Shader("shader/vertex.glsl","shader/fragment.glsl");
    Shader shader2("shader/vertex2.glsl","shader/fragment2.glsl");
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        //reader
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue)/2)+0.5;
        GLint vertexColorLocation = glGetUniformLocation(pshader->Program,"ourColor");
        
        
        pshader->Use();
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        shader2.Use();
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1,VAO);
    glDeleteBuffers(1,VBO);
    
    
    
    glfwDestroyWindow(window);
    delete pshader;
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}