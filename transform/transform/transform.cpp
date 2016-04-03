//
//  transform.cpp
//  transform
//
//  Created by tony on 16/4/1.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    GLfloat vertices1[] = {
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
    vbo = new VBO(vertices,36,GL_STATIC_DRAW,GL_FALSE,GL_TRUE);
    //ebo = new EBO(indices,6);
    vao->add_vbo(*vbo);
//    vao->enable();
//    ebo->enable();
//    vao->disable();
    
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
    
    
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(0.5,-0.5,0));
    //trans = glm::rotate(trans,glm::radians(90.0f),glm::vec3(0.0,0.0,1.0));
    //trans = glm::scale(trans, glm::vec3(0.5,0.5,0.5));
    //glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    
    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)win_width/win_height, 0.1f, 100.f);
    
    
    
    glEnable(GL_DEPTH_TEST);
    
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    
    
    
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        //reader
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        texture.use(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(pshader->Program,"ourTexture1"),0);
        texture2.use(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(pshader->Program,"ourTexture2"),1);
        
        //GLuint trans_loc = glGetUniformLocation(pshader->Program,"transform");
        //glUniformMatrix4fv(trans_loc,1,GL_FALSE,glm::value_ptr(trans));
        
        pshader->Use();
        
        GLuint model_loc = glGetUniformLocation(pshader->Program,"model");
        GLuint view_loc = glGetUniformLocation(pshader->Program,"view");
        GLuint projection_loc = glGetUniformLocation(pshader->Program,"projection");
        
        glUniformMatrix4fv(view_loc,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projection_loc,1,GL_FALSE,glm::value_ptr(projection));
        
        vao->enable();
        for(int i=0; i<10;i++)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * (i+1);
            model = glm::rotate(model, (GLfloat) glfwGetTime()* glm::radians(angle), glm::vec3(1.0f,1.0f,1.0f));
            glUniformMatrix4fv(model_loc,1,GL_FALSE,glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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