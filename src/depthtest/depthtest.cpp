//
//  depthtest.cpp
//  depthtest
//
//  Created by tony on 16/4/17.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "depthtest.hpp"

#include "gltools.hpp"
#include "camera.hpp"
#include <vector>
#include "Model.hpp"

const int win_width=800,win_height=600;


int last_x = win_width/2;
int last_y = win_height/2;

int mouse_xoffest,mouse_yoffest;



glm::vec3 cam_pos   = glm::vec3(0.0f, 0.0f,3.0f);
glm::vec3 light_pos(1.2f,1.0f,2.0f);
//glm::vec3 light_pos(0.0f,4.1f,0.0f);


Camera g_camera(cam_pos);

bool firstMouse = true;
int enter_window = 0;

bool keys[1024];
vector<bool> mouse_keys(10,false);



//fps
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
void do_movemen(GLfloat delta,GLfloat time){
    if(keys[GLFW_KEY_W])
        g_camera.ProcessKeyboard(FORWARD, delta);
    if(keys[GLFW_KEY_S])
        g_camera.ProcessKeyboard(BACKWARD, delta);
    if(keys[GLFW_KEY_A])
        g_camera.ProcessKeyboard(LEFT, delta);
    if(keys[GLFW_KEY_D])
        g_camera.ProcessKeyboard(RIGHT, delta);
    
    if(enter_window){
        if(mouse_keys[GLFW_MOUSE_BUTTON_LEFT])
            g_camera.ProcessCameraMove(mouse_xoffest, mouse_yoffest,delta);
    }
    
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if (action==GLFW_PRESS){
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE){
        keys[key] = false;
    }
    
}

static void mouse_callback(GLFWwindow * win, double posx,double posy)
{
    
    
    if(firstMouse)
    {
        last_x = posx;
        last_y = posy;
        firstMouse = false;
    }
    
    
    
    GLfloat xoffest = posx - last_x;
    GLfloat yoffest =last_y - posy;
    last_x = posx;
    last_y = posy;
    
    if(enter_window)
    {
        mouse_xoffest = xoffest;
        mouse_yoffest = yoffest;
        if(mouse_keys[GLFW_MOUSE_BUTTON_RIGHT])
            g_camera.ProcessMouseMove(xoffest, yoffest);
        
    }
    
    
}
static void mouse_click_callbakc(GLFWwindow* window, int button, int action, int mods){
    if(action==GLFW_PRESS){
        mouse_keys[button] = true;
    }
    if(action == GLFW_RELEASE){
        mouse_keys[button] = false;
    }
}

void scroll_callback(GLFWwindow* win, double xoffest, double yoffest)
{
    g_camera.ProcessMouseScrool(yoffest);
    
}

void cursor_enter_callback(GLFWwindow* window, int entered)
{
    enter_window = entered;
}


void init_callback(GLFWwindow* window){
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callbakc);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    
}


VAO *vao;
VBO *vbo;
EBO *ebo;
Shader *pshader;

VAO *light_vao;
VBO *light_vbo;
Shader *plightshader;
Model *pCube;
Model *pPlane;

void init_render()
{
    
    GLfloat vertices[] = {
        // Positions           // Normals      // Texture Coords
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,1.0f, 0.0f, 
        0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,1.0f, 1.0f, 
        0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,1.0f, 1.0f, 
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,1.0f, 0.0f, 
        0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,1.0f, 1.0f, 
        0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,1.0f, 1.0f, 
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,1.0f, 0.0f, 
        0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,1.0f, 1.0f, 
        0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,0.0f, 1.0f, 
        0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,0.0f, 1.0f, 
        0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,0.0f, 0.0f, 
        0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,1.0f, 0.0f, 
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,1.0f, 1.0f, 
        0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,1.0f, 0.0f, 
        0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,1.0f, 0.0f, 
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,1.0f, 1.0f, 
        0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,1.0f, 0.0f, 
        0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,1.0f, 0.0f, 
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    };
    
    GLfloat cubeVertices[] = {
        // Positions          // Texture Coords
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
    
    GLfloat planeVertices[] = {
        // Positions            // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
        5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        
        5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
    };
    
    //pmdole = new Model(vertices,36,"res/container2.png","res/container2_specular.png");
    pCube = new Model(cubeVertices,36,"res/marble.jpg","",GL_FALSE);
    pPlane = new Model(planeVertices,6,"res/metal.png","",GL_FALSE);
    
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
    
    //pmdole = new Model("res/nanosuit/nanosuit.obj");
    //pmdole = new Model("res/Miku/miku/gmiku.FBX");
    init_render();
    
    pshader = new Shader("lightmap.vert","lightmap.frag");
    
    
    //plightshader = new Shader("shader/vertex.glsl","shader/light_frag.glsl");
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    
    
    //matrix
    
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    
    
    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));
    //model = glm::scale(model, glm::vec3(0.2f,0.2f,0.2f));
    
    glm::mat4 light_model;
    
    
    glm::mat4 imatirx;
    
    
    
    
    
    update_mgr.push(do_movemen);
    
    
    
    while (!glfwWindowShouldClose(window)) {
        GLfloat curTime = glfwGetTime();
        deltaTime = curTime - lastTime;
        lastTime = curTime;
        
        glfwPollEvents();
        update_mgr.update(deltaTime, curTime);
        
        //reader
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        //        texture.use(GL_TEXTURE0);
        //        glUniform1i(glGetUniformLocation(pshader->Program,"ourTexture1"),0);
        //        texture2.use(GL_TEXTURE1);
        //        glUniform1i(glGetUniformLocation(pshader->Program,"ourTexture2"),1);
        
        
        
        light_pos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        light_pos.y = 1.0f + sin(glfwGetTime()) * 2.0f;
        //        printf("x:%f,y:%f,z:%f\n",light_pos.x,light_pos.y,light_pos.z);
        //        light_model = glm::translate(imatirx, light_pos);
        //        light_model = glm::scale(light_model, glm::vec3(0.2f));
        
        
        
        
        pshader->Use();
        
        
        
        
        //pshader->setUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
        //pshader->setUniform1i("material.specular", 1);
        //pshader->setUniform1f("material.shininess", 32.0f);
        
        
        //light
        //        pshader->setUniform3f("light.ambient", 0.2f,0.2f, 0.2f);
        //        pshader->setUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f);
        //        pshader->setUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
        pshader->setUniform3f("light.ambient", 1.0f, 1.0f, 1.0f);
        pshader->setUniform3f("light.diffuse", 1.0f, 1.0f, 1.0f);
        pshader->setUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
        //        //pos
        //        //pshader->setUniform3f("light.position", light_pos.x, light_pos.y, light_pos.z);
        pshader->setUniform3f("light.direction",-0.2f,-1.0f,-0.3f);
        pshader->setUniform3f("viewPos", cam_pos.x, cam_pos.y, cam_pos.z);
        
        
        
        
        
        //draw box
        model = glm::translate(imatirx, glm::vec3(-1.0f, 0.0f, -0.5f));
        update_matrix(pshader,model,view,projection);
        pCube->Draw(*pshader);
        
        model = glm::translate(imatirx, glm::vec3(-0.25f, 0.0f, -2.0f));
        update_matrix(pshader, model, view, projection);
        pCube->Draw(*pshader);
        
        model = imatirx;
        update_matrix(pshader, model, view, projection);
        pPlane->Draw(*pshader);
        
        
        //vao->enable();
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        //        for(int i=0;i<10;i++)
        //        {
        //            model = glm::translate(imatirx, cubePositions[i]);
        //            GLfloat t = glfwGetTime();
        //            GLfloat angle = 20.0f * i * t;
        //            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.3f,0.5f));
        //            pshader->setUniformMatrix4fv("model", glm::value_ptr(model));
        //            //matrix
        //
        //            glDrawArrays(GL_TRIANGLES, 0, 36);
        //            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //
        //        }
        
        
        
        //vao->disable();
        
        //draw light
        //        plightshader->Use();
        //        update_matrix(plightshader,light_model,view,projection);
        //
        //        light_vao->enable();
        //        glDrawArrays(GL_TRIANGLES,0,36);
        //        light_vao->disable();
        
        
        glfwSwapBuffers(window);
    }
    
    
    
    glfwDestroyWindow(window);
    delete pshader;
    
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}