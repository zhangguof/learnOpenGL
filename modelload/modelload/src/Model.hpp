//
//  Model.hpp
//  modelload
//
//  Created by tony on 16/4/14.
//  Copyright © 2016年 tony. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include <stdio.h>
#include <gltools.hpp>
#include "Mesh.hpp"
#include <vector>
#include <map>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(GLchar* path)
    {
        loadModel(path);
    }
    Model(GLfloat vertexs[],int count,string p1,string p2){
        loadSimpleModel(vertexs, count, p1, p2);
    }
    void Draw(Shader shader)
    {
        for(GLuint i=0;i<meshes.size();i++)
        {
            meshes[i].Draw(shader);
        }
    }
private:
    vector<Mesh> meshes;
    string directory;
    //map<string,Texture> texture_loaded;
    unordered_map<string, Texture> texture_loaded;
    
    
    void loadModel(string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate|aiProcess_FlipUVs);
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::"<<import.GetErrorString()<<endl;
        }
        directory = path.substr(0,path.find_last_of("/"));
        this->processNode(scene->mRootNode, scene);
    }
    
    void loadSimpleModel(GLfloat vertexs[],int count,string texture_diffuse_path,string texture_specular_path){
        vector<Vertex> vers;
        vector<GLuint> indices;
        vector<Texture> textures;
        directory = "res";
        int c=0;
        while(c<count)
        {
            Vertex v;
            int i = c<<3;
            v.Postion = glm::vec3(vertexs[i],vertexs[i+1],vertexs[i+2]);
            v.Normal = glm::vec3(vertexs[i+3],vertexs[i+4],vertexs[i+5]);
            v.TexCoords = glm::vec2(vertexs[i+6],vertexs[i+7]);
            vers.push_back(v);
            c++;
        }
        Texture t1,t2;
        t1.id = TEXTURE(texture_diffuse_path.c_str()).texture;
        t1.type = "texture_diffuse";
        t1.path = texture_diffuse_path;
        
        t2.id = TEXTURE(texture_specular_path.c_str()).texture;
        t2.type = "texture_specular";
        t2.path = texture_specular_path;
        
        textures.push_back(t1);
        textures.push_back(t2);
        
        
        Mesh m1(vers,indices,textures);
        meshes.push_back(m1);
    
        
        
    }
    void processNode(aiNode* node, const aiScene* scene)
    {
        for(GLuint i=0;i < node->mNumMeshes;i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(this->processMesh(mesh, scene));
        }
        for(GLuint i=0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }
    }
    Mesh processMesh(aiMesh * mesh, const aiScene* scene)
    {
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
        
        for(GLuint i=0; i< mesh->mNumVertices;i++)
        {
            //vertex normal texutreCoord
            Vertex vertex;
            vertex.Postion = glm::vec3(mesh->mVertices[i].x,
                                       mesh->mVertices[i].y,
                                       mesh->mVertices[i].z);
            vertex.Normal = glm::vec3(mesh->mNormals[i].x,
                                      mesh->mNormals[i].y,
                                      mesh->mNormals[i].z);
            
            if(mesh->mTextureCoords[0])
            {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                             mesh->mTextureCoords[0][i].y);
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f,0.0f);
                
            }
            vertices.push_back(vertex);
        }
        
        //indices
        for(GLuint i=0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(GLuint j=0;j<face.mNumIndices;j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        
        //material
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = this->loadMaterialTextures(material,
                                                                     aiTextureType_DIFFUSE,
                                                                     "texture_diffuse");
            textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
            vector<Texture> specularMaps = this->loadMaterialTextures(material,
                                                                     aiTextureType_SPECULAR,
                                                                     "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(),specularMaps.end());
        }
        
        return Mesh(vertices,indices,textures);
        
        
    }
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(GLuint i=0; i< mat->GetTextureCount(type);i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            const char* filename = str.C_Str();
            auto  t = texture_loaded.find(filename);
            
            if(t == texture_loaded.end())
            {
                Texture texture;
                string t_path = directory+"/"+filename;
                texture.id = TEXTURE(t_path.c_str()).texture;
                texture.type = typeName;
                texture.path = str;
                textures.push_back(texture);
                texture_loaded[filename] = texture;
                printf("load texutre:%s\n",filename);
                continue;
                
            }
            textures.push_back(t->second);
            printf("skip loaded texutre: %s\n",filename);

            
        }
        return textures;
        
    }
};




#endif /* Model_hpp */
