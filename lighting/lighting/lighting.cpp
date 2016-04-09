//
//  lighting.cpp
//  lighting
//
//  Created by tony on 16/4/6.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "lighting.hpp"
#include "gltools.hpp"
#include "camera.hpp"
#include <vector>

const int win_width=800,win_height=600;


int last_x = win_width/2;
int last_y = win_height/2;

int mouse_xoffest,mouse_yoffest;



glm::vec3 cam_pos   = glm::vec3(0.0f, 0.0f,3.0f);


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

void init_render()
{
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    
    
    vao = new VAO();
    vbo = new VBO(vertices,36,GL_STATIC_DRAW,GL_FALSE,GL_FALSE,GL_TRUE);
    //ebo = new EBO(indices,6);
    vao->add_vbo(*vbo);
//    vao->enable();
//    //ebo->enable();
//    vao->disable();
    
    light_vao = new VAO();
    //light_vbo = new VBO(vertices,36,GL_STATIC_DRAW,GL_FALSE,GL_TRUE,GL_TRUE);
    light_vao->add_vbo(*vbo);
    
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
    pshader = new Shader("shader/vertex.glsl","shader/fragment.glsl");
    
    plightshader = new Shader("shader/vertex.glsl","shader/light_frag.glsl");
    
    glEnable(GL_DEPTH_TEST);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //texture
    //TEXTURE texture("res/container.jpg");
    
    //TEXTURE texture2("res/awesomeface.png");
    
    //matrix
    
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    
    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));
    
    glm::mat4 light_model;
    glm::vec3 light_pos(1.2f,1.0f,2.0f);
    //glm::vec3 light_pos(0.0f,1.0f,0.0f);
    
    light_model = glm::translate(light_model, light_pos);
    light_model = glm::scale(light_model, glm::vec3(0.2f));
    

    
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
        
        
        
        
        
        
        
        
        pshader->Use();
        //color
        pshader->setUniform3f("objectColor", 1.0f, 1.0f, 1.0f);
        pshader->setUniform3f("lightColor", 1.0f, 0.5f, 0.31f);
        pshader->setUniform3f("lightPos", light_pos.x, light_pos.y, light_pos.z);
        
        
        //matrix
        update_matrix(pshader,model,view,projection);
        
        
        
        vao->enable();
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vao->disable();
        
        //draw light
        plightshader->Use();
        update_matrix(plightshader,light_model,view,projection);
        
        light_vao->enable();
        glDrawArrays(GL_TRIANGLES,0,36);
        light_vao->disable();
        
        
        glfwSwapBuffers(window);
    }
    
    
    
    glfwDestroyWindow(window);
    delete pshader;
    
    delete vao;
    delete vbo;
    
    
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}