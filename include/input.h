//
//  input.h
//  gltools
//
//  Created by tony on 16/4/23.
//  Copyright © 2016年 tony. All rights reserved.
//

#ifndef input_h
#define input_h
#include "gltools.hpp"
#include "camera.hpp"
#include <vector>

class Input
{
private:
    bool keys[1024];
    vector<bool> mouse_keys = vector<bool>(10,false);
    Camera *m_camera;
    int mouse_xoffest,mouse_yoffest;
    bool first_mouse = true;
    int last_x,last_y;
    
    int enter_window = 0;

    
    
    Input(){}
    
public:
    static Input *m_inst;
    static Input* getInst(){
        if(m_inst == NULL)
        {
            m_inst = new Input();
        }
        return m_inst;
    }
    
    void init(Camera* cam){
        getInst()->m_camera = cam;
    }
    void init_callbakc(GLFWwindow* window)
    {
        glfwSetKeyCallback(window, key_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetMouseButtonCallback(window, mouse_click_callback);
        glfwSetCursorEnterCallback(window, cursor_enter_callback);
    }
    static void do_movemnet(GLfloat delta, GLfloat time){
        Input *inst = getInst();
        auto cam = inst->m_camera;
        if(inst->keys[GLFW_KEY_W])
            cam->ProcessKeyboard(FORWARD, delta);
        if(inst->keys[GLFW_KEY_S])
            cam->ProcessKeyboard(BACKWARD, delta);
        if(inst->keys[GLFW_KEY_A])
            cam->ProcessKeyboard(LEFT, delta);
        if(inst->keys[GLFW_KEY_D])
            cam->ProcessKeyboard(RIGHT, delta);
        
        if(inst->enter_window){
            if(inst->mouse_keys[GLFW_MOUSE_BUTTON_LEFT])
                cam->ProcessCameraMove(inst->mouse_xoffest, inst->mouse_yoffest,delta);
        }
    }
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto inst = getInst();
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        
        if (action==GLFW_PRESS){
            inst->keys[key] = true;
        }
        else if (action == GLFW_RELEASE){
            inst->keys[key] = false;
        }
    }
    
    static void mouse_callback(GLFWwindow* win, double posx, double posy){
        auto inst = getInst();
        if(inst->first_mouse){
            inst->last_x = posx;
            inst->last_y = posy;
            inst->first_mouse = false;
        }
        
        GLfloat xoffest = posx - inst->last_x;
        GLfloat yoffest = posy - inst->last_y;
        inst->last_x = posx;
        inst->last_y = posy;
        
        if (inst->enter_window)
        {
            inst->mouse_xoffest = xoffest;
            inst->mouse_yoffest = yoffest;
            if(inst->mouse_keys[GLFW_MOUSE_BUTTON_RIGHT])
                inst->m_camera->ProcessMouseMove(xoffest, yoffest);
        }
    }
    
    static void mouse_click_callback(GLFWwindow* windows,int button, int action, int mods)
    {
        auto inst = getInst();
        if(action==GLFW_PRESS){
            inst->mouse_keys[button] = true;
        }
        if(action == GLFW_RELEASE){
            inst->mouse_keys[button] = false;
        }
    }
    static void scroll_callback(GLFWwindow* win, double xoffest, double yoffest)
    {
        getInst()->m_camera->ProcessMouseScrool(yoffest);
        
    }
    
    static void cursor_enter_callback(GLFWwindow* window, int entered)
    {
        getInst()->enter_window = entered;
    }
};

Input* Input::m_inst = NULL;




#endif /* input_h */
