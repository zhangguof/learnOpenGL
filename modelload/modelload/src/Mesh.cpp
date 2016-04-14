//
//  Mesh.cpp
//  modelload
//
//  Created by tony on 16/4/13.
//  Copyright © 2016年 tony. All rights reserved.
//

#include <gltools.hpp>
#include <vector>
#include "Mesh.hpp"


Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->setupMesh();
        
        
}

void Mesh::Draw(Shader shader){
    GLuint diffuseNr = 1;
    GLuint specualNr = 1;
    for(GLuint i=0; i< textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        stringstream ss;
        string number;
        string name = textures[i].type;
        
        if(name == "texture_diffuse")
            ss << diffuseNr++;
        else if (name=="texture_specular")
            ss << specualNr++;
        number = ss.str();
        //cout<<"type name:"<<name<<":"<<number<<endl;
        //glUniform1i(glGetUniformLocation(shader.Program,("material."+name+number).c_str()),i);
        shader.setUniform1i(("material."+name+number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    //glUniform1f(glGetUniformLocation(shader.Program,"materail.shininess"),16.0f);
    shader.setUniform1f("material.shininess", 16.0f);
    
    //Draw
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    for(GLuint i=0; i < textures.size();i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
}

void Mesh::setupMesh(){
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),
                 &this->vertices[0],GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint),
                 &this->indices[0],GL_STATIC_DRAW);
    
    
    //postion
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)0);
    
    //noraml
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, Normal));
    
    //coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, TexCoords));
    
    glBindVertexArray(0);
    
}

