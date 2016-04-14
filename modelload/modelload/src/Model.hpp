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
            Texture texture;
            string t_path = directory+"/"+str.C_Str();
            texture.id = TEXTURE(t_path.c_str()).texture;
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            
        }
        return textures;
        
    }
};




#endif /* Model_hpp */
