//
//  textRendering.cpp
//  TextRendering
//
//  Created by tony on 16/4/23.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "textRendering.hpp"
//
//  depthtest.cpp
//  depthtest
//
//  Created by tony on 16/4/17.
//  Copyright © 2016年 tony. All rights reserved.
//


#include "gltools.hpp"
#include "camera.hpp"
#include <vector>
#include "Model.hpp"
#include "input.h"

const int win_width=800,win_height=600;


int last_x = win_width/2;
int last_y = win_height/2;

int mouse_xoffest,mouse_yoffest;



glm::vec3 cam_pos   = glm::vec3(0.0f, 0.0f,3.0f);
glm::vec3 light_pos(1.2f,1.0f,2.0f);

Camera g_camera(cam_pos);
Input* input;



//fps
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}


void init_callback(GLFWwindow* window){
    input->init(&g_camera);
    glfwSetErrorCallback(error_callback);
    input->init_callbakc(window);
    
}



void init_render()
{
    
    
}

typedef void(* update_func_type)(GLfloat,GLfloat);

class UpdateMgr{
    vector<update_func_type> funcs;
public:
    void push(update_func_type f){
        funcs.push_back(f);
    }
    void update(GLfloat delta,GLfloat time){
        unsigned int size = funcs.size();
        for(int i=0;i < size;i++){
            funcs[i](delta,time);
        }
    }
    
};
UpdateMgr update_mgr;


void update_matrix(Shader *ps,glm::mat4 &model,glm::mat4 &view,
                   glm::mat4 &projection)
{
    //matrix
    
    ps->setUniformMatrix4fv("model", glm::value_ptr(model));
    
    view = g_camera.GetViewMatrix();
    ps->setUniformMatrix4fv("view", glm::value_ptr(view));
    
    projection = glm::perspective(glm::radians(g_camera.Zoom),
                                  (GLfloat)win_width/win_height,
                                  0.1f, 100.f);
    ps->setUniformMatrix4fv("projection", glm::value_ptr(projection));
    
}






int main(int argc, char **argv)
{
    
    printf("start opengl\n");
    
    
    GLFWwindow* window = create_window(win_width,win_height,
                                       "3Dexample");
    
    init_callback(window);
    
    init_render();
    
    update_mgr.push(input->do_movemnet);
    
    
    
    while (!glfwWindowShouldClose(window)) {
        GLfloat curTime = glfwGetTime();
        deltaTime = curTime - lastTime;
        lastTime = curTime;
        
        glfwPollEvents();
        update_mgr.update(deltaTime, curTime);
        
        //reader
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        
        
        glfwSwapBuffers(window);
    }
    
    
    
    glfwDestroyWindow(window);
    
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}