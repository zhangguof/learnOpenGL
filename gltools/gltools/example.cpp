//
//  example.cpp
//  gltools
//
//  Created by tony on 16/4/2.
//  Copyright © 2016年 tony. All rights reserved.
//

#include <stdio.h>
#include "gltools.hpp"

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


VAO *vao;
VBO *vbo;
EBO *ebo;
Shader *pshader;

void init_render()
{
//    GLfloat vertices[]={
//        // 三角形       //color        //texutre coord
//        -0.5f, -0.5f, 0.0f,   1.0f,0.0f,0.0f, 0.0f,0.0f,
//        0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f, 1.0f, 0.0f,
//        0.0f,  0.5f, 0.0f, 0.0f,0.0f,1.0f, 0.5f, 1.0f,
//    };
    
    GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    
    vao = new VAO();
    vbo = new VBO(vertices,4);
    ebo = new EBO(indices,6);
    vao->add_vbo(*vbo);
    vao->enable();
    ebo->enable();
    vao->disable();
    
}




int main(int argc, char **argv)
{
    
    printf("start opengl\n");
    
    
    glfwSetErrorCallback(error_callback);
    GLFWwindow* window = create_window(win_width,win_height,
                                       "3Dexample");
    
    glfwSetKeyCallback(window, key_callback);
    
    init_render();
    pshader = new Shader("shader/vertex.glsl","shader/fragment.glsl");
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //texture
    TEXTURE texture("container.jpg");

    
    TEXTURE texture2("awesomeface.png");
    




    
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        //reader
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        texture.use(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(pshader->Program,"ourTexture1"),0);
        texture2.use(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(pshader->Program,"ourTexture2"),1);
        pshader->Use();


        vao->enable();
        
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vao->disable();
        
        
        glfwSwapBuffers(window);
    }
    
    
    
    glfwDestroyWindow(window);
    delete pshader;
    
    delete vao;
    delete vbo;
    
    
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}