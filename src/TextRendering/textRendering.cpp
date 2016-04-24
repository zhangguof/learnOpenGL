//
//  textRendering.cpp
//  TextRendering
//
//  Created by tony on 16/4/23.
//  Copyright © 2016年 tony. All rights reserved.
//

#include "textRendering.hpp"


#include "gltools.hpp"
#include "camera.hpp"
#include "Model.hpp"
#include "input.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

const int win_width=800,win_height=600;


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
    
    glfwSetErrorCallback(error_callback);
    
    input->init(&g_camera);
    input->init_callbakc(window);
    
    
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

FT_Library ft;
FT_Face face;

struct Character{
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

std::map<GLulong, Character> Characters;
//std::map<GLulong, Character> UnicodeCharcters;

//utf-8 to unicode
GLulong get_unicode(const char *s, size_t &idx)
{
    GLubyte b0 = s[idx];
    GLulong re;
    if(!(b0 & 0x80))
    {
        idx++;
        return b0;
    }
    if((b0&0xE0) == 0xC0)
    {
        re = ((b0&0x1F)<<6) + (s[idx+1]&0x3F); //110xxxx 10xxxxxx
        idx += 2;
        return re;
    }
    if((b0&0xF0) == 0xE0)
    {
        //1110xxxx 10xxxxxx 10xxxxxx
        re = ((b0&0x0F)<<12) + ((s[idx+1]&0x3F)<<6) + ((s[idx+2]&0x3F));
        idx += 3;
        return re;
    }
    
    //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    re = ((b0&0x07)<<18) + ((s[idx+1]&0x3F)<<12) + ((s[idx+2]&0x3F)<<6) + ((s[idx+3]&0x3F));
    idx += 4;
    return re;
    
}

void add_font_texutre(GLulong c)
{
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
        std::cout<<"ERROR::FREETYPE:Failed to load GLyph"<<std::endl;
        return;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    
    // 设置纹理选项
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left,face->glyph->bitmap_top),
        (GLuint)(face->glyph->advance.x)
    };
    
    Characters.insert(std::pair<GLulong,Character>(c,character));
    
}


void init_font(const char* font_path)
{
    if(FT_Init_FreeType(&ft))
        std::cout<<"ERROR::FREETYPE: Could not init FreeType Libary"<<std::endl;
    
    if(FT_New_Face(ft, font_path, 0, &face))
        std::cout<<"ERROR::FREETYTE: Failed to load font file."<<std::endl;
    
    int error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    if(error)
        std::cout<<"ERROR::FREETYTE: select charmap file."<<std::endl;
    
    FT_Set_Pixel_Sizes(face, 0, 30);

    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    for(GLubyte c=0;c<128;c++)
//    {
//        add_font_texutre(c);
//    }
//    if (FT_Load_Char(face, 0x54c8, FT_LOAD_RENDER))
//    {
//        std::cout<<"ERROR::FREETYPE:Failed to load GLyph"<<std::endl;
//    }
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
}

GLuint VAO,VBO;
void init_render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    init_font("res/NotoSansCJKsc-Black.otf");
    
    
    
}

void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color,glm::mat4 projection)
{
    s.Use();
    //glUniform3f(glGetUniformLocation(s.Program, "textColor"),color.x,color.y,color.z);
    s.setUniform3f("textColor", color.x, color.y, color.z);
    s.setUniformMatrix4fv("projection", glm::value_ptr(projection));
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    size_t idx = 0;
    size_t len = text.size();
    const char* text_s = text.c_str();
    while(idx<len)
    {
        //GLulong c = text[idx];
        GLulong c = get_unicode(text_s, idx);
        
        auto it = Characters.find(c);
        if(it == Characters.end())
        {
            add_font_texutre(c);
        }
        Character ch = Characters[c];
        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y)*scale;
        
        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        
        //vbo
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},
            {xpos, ypos,     0.0,1.0},
            {xpos+w,ypos,    1.0,1.0},
            
            {xpos,ypos+h,   0.0, 0.0},
            {xpos+w, ypos, 1.0,1.0},
            {xpos+w,ypos+h,1.0,0.0}
        };
        
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        x += (ch.Advance>>6) * scale; // 1/64 pixel
        
        //idx++;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void clearRC()
{
    //reader
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Render(){
    
}




int main(int argc, char **argv)
{
    
    printf("start opengl\n");
    
    
    GLFWwindow* window = create_window(win_width,win_height,
                                       "RenderText");
    
    init_callback(window);
    
    init_render();
    
    update_mgr.push(input->do_movemnet);
    
    Shader shader("font.vs","font.frag");
    
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    
    
    while (!glfwWindowShouldClose(window)) {
        GLfloat curTime = glfwGetTime();
        deltaTime = curTime - lastTime;
        lastTime = curTime;
        
        glfwPollEvents();
        update_mgr.update(deltaTime, curTime);
        
        clearRC();
        
        Render();
        //RenderText(shader, "Open GL test.", 0, 0, 1, glm::vec3(1.0,1.0,1.0),projection);
        RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f),projection);
        RenderText(shader, "这里是中文！", 25.0f, 75.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f),projection);
        
        //RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f),projection);

        
        
        glfwSwapBuffers(window);
    }
    
    
    
    glfwDestroyWindow(window);
    
    
    
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}