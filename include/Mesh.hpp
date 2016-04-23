//
//  Mesh.hpp
//  modelload
//
//  Created by tony on 16/4/14.
//  Copyright © 2016年 tony. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <gltools.hpp>
#include <vector>
#include <string>
#include <assimp/types.h>

struct Vertex{
    glm::vec3 Postion;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    string type;
    aiString path;
};

class Mesh{
public:
    //Mesh data
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    void Draw(Shader shader);
    
private:
    GLuint VAO,VBO,EBO;
    void setupMesh();

};


#endif /* Mesh_hpp */
