//
//  camera.hpp
//  camera
//
//  Created by tony on 16/4/4.
//  Copyright © 2016年 tony. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_MOVE{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

const GLfloat YAW           = -90.0f;
const GLfloat PITCH         = 0.0f;
const GLfloat SPEED         = 3.0f;
const GLfloat SENSITIVITY   = 0.25f;
const GLfloat ZOOM          = 45.0f;

class Camera
{
public:
    glm::vec3 Postion;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    GLfloat Yaw;
    GLfloat Pitch;
    
    GLfloat MoveSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;
    
    Camera(glm::vec3 postion = glm::vec3(0.0f,0.0f,0.0f),
           glm::vec3 up = glm::vec3(0.0,1.0f,0.0f),
           GLfloat yaw = YAW,GLfloat pitch =PITCH):Front(glm::vec3(0.0,0.0f,-1.0f)),Zoom(ZOOM),
                                                   MoveSpeed(SPEED),MouseSensitivity(SENSITIVITY)
    {
        Postion = postion;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        
        printf("pos:%f,%f,%f\n",Postion.x,Postion.y,Postion.z);
        UpdateCamera();
        printf("pos:%f,%f,%f\n",Postion.x,Postion.y,Postion.z);
        printf("yaw:%f,pitch:%f\n",Yaw,Pitch);
        printf("up:%f,%f,%f\n",Up.x,Up.y,Up.z);
        printf("front:%f,%f,%f\n",Front.x,Front.y,Front.z);

        
        
        
    }
    
    Camera(GLfloat posx, GLfloat posy, GLfloat posz, GLfloat upx,GLfloat upy,GLfloat upz,
           GLfloat yaw,GLfloat pitch):Front(glm::vec3(0.0,0.0f,-1.0f)),Zoom(ZOOM),
                                      MoveSpeed(SPEED),MouseSensitivity(SENSITIVITY)
    {
        Postion = glm::vec3(posx,posy,posz);
        WorldUp = glm::vec3(upx,upy,upz);
        Yaw = yaw;
        Pitch = pitch;
        UpdateCamera();
    }
    
    glm::mat4 GetViewMatrix(){
        return glm::lookAt(Postion, Postion+Front, Up);
    }
    
    void ProcessKeyboard(Camera_MOVE dir, GLfloat deltaTime){
        GLfloat velocity = MoveSpeed * deltaTime;
        if (dir == FORWARD)
            Postion += Front * velocity;
        if (dir == BACKWARD)
            Postion -= Front * velocity;
        if(dir == LEFT)
            Postion -= Right * velocity;
        if(dir == RIGHT)
            Postion += Right * velocity;
    }
    
    void ProecessMouseMove(GLfloat xoffest, GLfloat yoffest,
                           GLboolean constrainPitch = true)
    {
        xoffest *= MouseSensitivity;
        yoffest *= MouseSensitivity;
        
        Yaw += xoffest;
        Pitch += yoffest;
        if(constrainPitch){
            if(Pitch > 89.0f)
                Pitch = 89.0f;
            if(Pitch < -89.0f)
                Pitch = -89.0f;
        }
        UpdateCamera();
        
    }
    
    void ProcessMouseScrool(GLfloat yoffest)
    {
        Zoom -= yoffest;
        if(Zoom <= 1.0f)
            Zoom = 1.0f;
        if(Zoom >= 45.0f)
            Zoom = 45.0f;
    }
    
private:
    void UpdateCamera()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        front.y = sin(glm::radians(Pitch));
        front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        Front = glm::normalize(front);
        
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right,Front));
    }
};




#endif /* camera_hpp */
