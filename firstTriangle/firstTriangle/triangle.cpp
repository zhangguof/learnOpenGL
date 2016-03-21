//
//  triangle.cpp
//  firstTriangle
//
//  Created by tony on 16/3/21.
//  Copyright © 2016年 tony. All rights reserved.
//


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <fstream>




const int win_width = 640;
const int win_height = 480;






void read_shader_file(const char *fname,char *buf){
    int fp = open(fname,O_RDONLY);
    read(fp, buf, 2048);
}

GLuint init_shader(GLuint *shaders)
{
    char shader_src_buf[2048];
    const char *buf = shader_src_buf;
    
    //vertex shader
    read_shader_file("shader/vertextShader.glsl",shader_src_buf);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&buf,NULL);
    glCompileShader(vertexShader);
    
    GLint success;
    GLchar infolog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,infolog);
        std::cout<< "ERROR:SHADER::VERTEX::COMPILATION_FAILD\n"<<infolog<<std::endl;
        return NULL;
        
    }
    //fragment shader
    read_shader_file("shader/fragmentShader.glsl", shader_src_buf);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,(const char* const*) &buf,NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader,512,NULL,infolog);
        std::cout<< "ERROR:SHADER::FRAGMENT::COMPILATION_FAILD\n"<<infolog<<std::endl;
        return NULL;
        
    }
    
    //fragment shader2
    read_shader_file("shader/fragmentShader2.glsl", shader_src_buf);
    GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2,1,(const char* const*) &buf,NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader2,512,NULL,infolog);
        std::cout<< "ERROR:SHADER::FRAGMENT::COMPILATION_FAILD\n"<<infolog<<std::endl;
        return NULL;
        
    }
    
    GLuint shaderProgram = glCreateProgram();
    GLuint shaderProgram2 = glCreateProgram();
    
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,NULL,infolog);
        std::cout<< "ERROR:SHADER::PROGRAM::LINK_FAILD\n"<<infolog<<std::endl;
        return NULL;
    }
    
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    
    glGetProgramiv(shaderProgram2,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(shaderProgram2,512,NULL,infolog);
        std::cout<< "ERROR:SHADER::PROGRAM::LINK_FAILD\n"<<infolog<<std::endl;
        return NULL;
    }
    
    //glUseProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);
    
    shaders[0] = shaderProgram;
    shaders[1] = shaderProgram2;
    
    return 2;
    
    
    
    
}

GLuint VBO;
GLuint VAO;

GLuint VAO2;
GLuint VBO2;

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

void init_render(GLuint *vao, GLuint *vbo, GLuint *shaders)
{
    GLfloat vertices[] = {
        // 第一个三角形
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, 0.5f, 0.0f,  // 左上角
    };
    GLfloat vertices2[]={
        // 第二个三角形
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    
    
    gen_vao_vbo(vao, vbo, vertices,sizeof(vertices));
    gen_vao_vbo(&VAO2, &VBO2, vertices2, sizeof(vertices2));
    
    
    init_shader(shaders);
    
    
}


void render(){
    
    
    
}


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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
    
    //glViewport(0, 0, win_width, win_height);
    
    const char *version = (const char *)glGetString(GL_VERSION);
    const char *vendor = (const char *)glGetString(GL_VENDOR);
    printf("OpenGL version:%s\nOpengl Vendor:%s\n",version,vendor);
    char buf[1024];
    getcwd(buf,sizeof(buf));
    printf("cwd:%s\n",buf);
    
    GLuint shader_program[2];
    init_render(&VAO, &VBO, shader_program);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        //reader
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader_program[0]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        glUseProgram(shader_program[1]);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    
    
    
    glfwDestroyWindow(window);
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    
    
}
