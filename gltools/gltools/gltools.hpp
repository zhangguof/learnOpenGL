//
//  gltools.hpp
//  gltools
//
//  Created by tony on 16/4/2.
//  Copyright © 2016年 tony. All rights reserved.
//

#ifndef gltools_hpp
#define gltools_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

GLFWwindow* create_window(int width, int height,
                          const char *titile, GLFWmonitor *monitor=NULL, GLFWwindow* share=NULL);

const GLuint VBOCORlORIDX = 3;
const GLuint VBOTEXCOORDIDX = 6;
//const GLuint POS_LOC = 0;
//const GLuint COL_LOC = 1;
//const GLuint TEX_LOC = 2;
//const GLuint NOR_LOC = 3;

class VBO{
    GLuint vbo;
    GLboolean enableCorlor;
    GLboolean enableTexcoord;
    GLboolean enableNormal;
    GLfloat *vertices;
    GLuint num;
    GLenum draw_usage;
    GLuint strip;
public:
    VBO(GLfloat verts[], GLuint n,GLenum usage=GL_STATIC_DRAW,
        GLboolean enable_corlor = GL_TRUE,
        GLboolean enable_texcoord = GL_TRUE,
        GLboolean enable_normal = GL_TRUE){
        
        enableCorlor = enable_corlor;
        enableTexcoord = enable_texcoord;
        enableNormal = enable_normal;
        vertices = verts;
        draw_usage = usage;
        num = n;
        
        strip = 3;
        if(enableCorlor){
            strip += 3;
        }
        if(enableTexcoord){
            strip += 2;
        }
        if(enableNormal)
        {
            strip += 3;
        }
        
        glGenBuffers(1,&vbo);

        
    }
    ~VBO(){
        printf("delete vbo...\n");
        glDeleteBuffers(1,&vbo);
    }

    void bindVBO();
};

class EBO{
    GLuint ebo;
public:
    EBO(GLuint indices[],GLuint num,GLenum usage=GL_STATIC_DRAW){
        glGenBuffers(1,&ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,num * sizeof(GLuint),indices,usage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
    
    void enable(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    }
    void disable(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
};


class VAO{
    GLuint vao;
public:
    VAO(){
        glGenVertexArrays(1,&vao);
        
    }
    ~VAO(){
        printf("delete vao...\n");
        glDeleteVertexArrays(1,&vao);
    }
public:
    void add_vbo(VBO &vbo){
        glBindVertexArray(vao);
        vbo.bindVBO();
        glBindVertexArray(0);
    }
    
    void enable(){
        glBindVertexArray(vao);
    }
    void disable(){
        glBindVertexArray(0);
    }


    
};

class TEXTURE{
    GLuint texture;
public:
    TEXTURE(const char * filename){
        int w,h;
        unsigned char* image = SOIL_load_image(filename,&w,&h,0,SOIL_LOAD_RGB);
        if(!image){
            printf("error can't read file:%s\n",filename);
            return;
        }
        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        //    // Set the texture wrapping parameters
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //    // Set texture filtering parameters
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void use(GLenum tex=GL_TEXTURE0){
        glActiveTexture(tex);
        glBindTexture(GL_TEXTURE_2D,texture);
    }

};

#endif /* gltools_hpp */

