#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <learnopengl/mesh.h>
#include <learnopengl/shader.h>

#include <stdio.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
    /*  Model Data */
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;

    vector<Mesh> Wheel_01_meshes;
    vector<Mesh> Wheel_03_meshes;

    vector<Mesh> Wheel_02_meshes;
    vector<Mesh> Wheel_04_meshes;

    vector<Mesh> Door_L1_meshes;
    vector<Mesh> Door_L2_meshes;
    vector<Mesh> Door_R1_meshes;
    vector<Mesh> Door_R2_meshes;

    vector<Mesh> Door_B_meshes;


    string directory;
    bool gammaCorrection;
    int num = 0;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++){
            meshes[i].Draw(shader);
        }
    }

    // draws the model, and thus all its meshes
    void Draw_Car(Shader shader, float angle, bool door_L1, bool door_L2, bool door_R1, bool door_R2, bool boor_B)
    {
        static float goOrBackAngle = 0.0f;

        float _angle;

        //cout << "angle = " << angle << endl;
        //cout << "door_L1 = " << door_L1 << " door_L2 = " << door_L2 << " door_R1 = " << door_R1 << " door_R2 = " << door_R2 << " boor_B = " << boor_B << endl;

        if(angle >= -46.0f && angle <= 46.0f){
            goOrBackAngle -= 1.0f;
            _angle = -angle;
        }else if (angle >= 134.0f && angle <= 226.0f){
            goOrBackAngle += 1.0f;
            _angle = -(angle-180.0f);
        }else{
            assert(0);
        }

        // cout << "meshes.size():: " << meshes.size() << endl;
        // cout << "Wheel_01_meshes.size():: " << Wheel_01_meshes.size() << endl;
        // cout << "Wheel_02_meshes.size():: " << Wheel_02_meshes.size() << endl;
        // cout << "Wheel_03_meshes.size():: " << Wheel_03_meshes.size() << endl;
        // cout << "Wheel_04_meshes.size():: " << Wheel_04_meshes.size() << endl;
        // cout << "Door_L1_meshes.size():: " << Door_L1_meshes.size() << endl;
        // cout << "Door_L2_meshes.size():: " << Door_L2_meshes.size() << endl;
        // cout << "Door_R1_meshes.size():: " << Door_R1_meshes.size() << endl;
        // cout << "Door_R2_meshes.size():: " << Door_R2_meshes.size() << endl;
        // cout << "Door_B_meshes.size():: " << Door_B_meshes.size() << endl;
        // cout << endl;
        // cout << endl;
        // cout << endl;

        // meshes.size():: 29532
        // Wheel_01_meshes.size():: 1256
        // Wheel_02_meshes.size():: 1256
        // Wheel_03_meshes.size():: 1257
        // Wheel_04_meshes.size():: 1257
        // Door_L1_meshes.size():: 626
        // Door_L2_meshes.size():: 587
        // Door_R1_meshes.size():: 626
        // Door_R2_meshes.size():: 585
        // Door_B_meshes.size():: 2362

        for(unsigned int i = 0; i < meshes.size(); i++){
            meshes[i].Draw(shader);
        }

        if(door_L1){
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            model = glm::translate(model, glm::vec3(81.0f, 101.0f, -89.0f));
            model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(-81.0f, -101.0f, 89.0f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_L1_meshes.size(); i++){
                Door_L1_meshes[i].Draw(shader);
            }
        }else{
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_L1_meshes.size(); i++){
                Door_L1_meshes[i].Draw(shader);
            }
        }

        if(door_L2){
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            model = glm::translate(model, glm::vec3(-28.0f, 110.0f, -89.0f));
            model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(28.0f, -110.0f, 89.0f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_L2_meshes.size(); i++){
                Door_L2_meshes[i].Draw(shader);
            }
        }else{
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_L2_meshes.size(); i++){
                Door_L2_meshes[i].Draw(shader);
            }
        }

        if(door_R1){
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            model = glm::translate(model, glm::vec3(81.0f, 101.0f, 89.0f));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(-81.0f, -101.0f, -89.0f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_R1_meshes.size(); i++){
                Door_R1_meshes[i].Draw(shader);
            }
        }else{
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_R1_meshes.size(); i++){
                Door_R1_meshes[i].Draw(shader);
            }
        }


        if(door_R2){
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            model = glm::translate(model, glm::vec3(-28.0f, 110.0f, 89.0f));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(28.0f, -110.0f, -89.0f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_R2_meshes.size(); i++){
                Door_R2_meshes[i].Draw(shader);
            }
        }else{
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_R2_meshes.size(); i++){
                Door_R2_meshes[i].Draw(shader);
            }
        }


        if(boor_B){
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            model = glm::translate(model, glm::vec3(-169.0f, 164.0f, 0.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(169.0f, -164.0f, 0.0f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_B_meshes.size(); i++){
                Door_B_meshes[i].Draw(shader);
            }
        }else{
            glm::mat4 model = glm::mat4(1.0);
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            shader.setMat4("model", model);
            for(unsigned int i = 0; i < Door_B_meshes.size(); i++){
                Door_B_meshes[i].Draw(shader);
            }
        }

        glm::mat4 model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        model = glm::translate(model, glm::vec3(131.0f, 38.0f, -89.0f));
        model = glm::rotate(model, glm::radians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(goOrBackAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-131.0f, -38.0f, 89.0f));
        shader.setMat4("model", model);
        for(unsigned int i = 0; i < Wheel_01_meshes.size(); i++){
            Wheel_01_meshes[i].Draw(shader);
        }

        model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        model = glm::translate(model, glm::vec3(131.0f, 38.0f, 89.0f));
        model = glm::rotate(model, glm::radians(_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(goOrBackAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-131.0f, -38.0f, -89.0f));
        shader.setMat4("model", model);
        for(unsigned int i = 0; i < Wheel_03_meshes.size(); i++){
            Wheel_03_meshes[i].Draw(shader);
        }

        model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        model = glm::translate(model, glm::vec3(-134.0f, 38.0f, -89.0f));
        model = glm::rotate(model, glm::radians(goOrBackAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(134.0f, -38.0f, 89.0f));
        shader.setMat4("model", model);
        for(unsigned int i = 0; i < Wheel_01_meshes.size(); i++){
            Wheel_02_meshes[i].Draw(shader);
        }

        model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        model = glm::translate(model, glm::vec3(-134.0f, 38.0f, -89.0f));
        model = glm::rotate(model, glm::radians(goOrBackAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(134.0f, -38.0f, 89.0f));
        shader.setMat4("model", model);
        for(unsigned int i = 0; i < Wheel_01_meshes.size(); i++){
            Wheel_04_meshes[i].Draw(shader);
        }
    }

private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        //cout << "node->mName :: " << node->mName.C_Str() << endl;

        static int  Wheel_01_flag = 0;
        static int  Wheel_03_flag = 0;
        static int  Wheel_02_flag = 0;
        static int  Wheel_04_flag = 0;
        static int  Door_L1_flag = 0;
        static int  Door_L2_flag = 0;
        static int  Door_R1_flag = 0;
        static int  Door_R2_flag = 0;
        static int  Door_B_flag = 0;

        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];


            if((Wheel_01_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Wheel_01") == 0){
                Wheel_01_meshes.push_back(processMesh(mesh, scene));
                Wheel_01_flag = 1;
                Wheel_02_flag = Wheel_03_flag = Wheel_04_flag = Door_L1_flag = Door_L2_flag = Door_R1_flag = Door_R2_flag = Door_B_flag = 0;
            }else if((Wheel_03_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Wheel_03") == 0){
                Wheel_03_meshes.push_back(processMesh(mesh, scene));
                Wheel_03_flag = 1;
                Wheel_01_flag = Wheel_02_flag = Wheel_04_flag = Door_L1_flag = Door_L2_flag = Door_R1_flag = Door_R2_flag = Door_B_flag = 0;
            }else if((Wheel_02_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Wheel_02") == 0){
                Wheel_02_meshes.push_back(processMesh(mesh, scene));
                Wheel_02_flag = 1;
                Wheel_01_flag = Wheel_03_flag = Wheel_04_flag = Door_L1_flag = Door_L2_flag = Door_R1_flag = Door_R2_flag = Door_B_flag = 0;
            }else if((Wheel_04_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Wheel_04") == 0){
                Wheel_04_meshes.push_back(processMesh(mesh, scene));
                Wheel_04_flag = 1;
                Wheel_01_flag = Wheel_02_flag = Wheel_03_flag = Door_L1_flag = Door_L2_flag = Door_R1_flag = Door_R2_flag = Door_B_flag = 0;
            }else if((Door_L1_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Door_L1") == 0){
                Door_L1_meshes.push_back(processMesh(mesh, scene));
                Door_L1_flag = 1;
                Wheel_01_flag = Wheel_02_flag = Wheel_03_flag = Wheel_04_flag = Door_L2_flag = Door_R1_flag = Door_R2_flag = Door_B_flag = 0;
            }else if((Door_L2_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Door_L2") == 0){
                Door_L2_meshes.push_back(processMesh(mesh, scene));
                Door_L2_flag = 1;
                Wheel_01_flag = Wheel_02_flag = Wheel_03_flag = Wheel_04_flag = Door_L1_flag = Door_R1_flag = Door_R2_flag = Door_B_flag = 0;
            }else if((Door_R1_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Door_R1") == 0){
                Door_R1_meshes.push_back(processMesh(mesh, scene));
                Door_R1_flag = 1;
                Wheel_01_flag = Wheel_02_flag = Wheel_03_flag = Wheel_04_flag = Door_L1_flag = Door_L2_flag = Door_R2_flag = Door_B_flag = 0;
            }else if((Door_R2_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Door_R2") == 0){
                Door_R2_meshes.push_back(processMesh(mesh, scene));
                Door_R2_flag = 1;
                Wheel_01_flag = Wheel_02_flag = Wheel_03_flag = Wheel_04_flag = Door_L1_flag = Door_L2_flag = Door_R1_flag = Door_B_flag = 0;
            }else if((Door_B_flag == 1 && strcmp(node->mName.C_Str(), "defaultobject") == 0) || strcmp(node->mName.C_Str(), "Door_B") == 0){
                Door_B_meshes.push_back(processMesh(mesh, scene));
                Door_B_flag = 1;
                Wheel_01_flag = Wheel_02_flag = Wheel_03_flag = Wheel_04_flag = Door_L1_flag = Door_L2_flag = Door_R1_flag = Door_R2_flag = 0;
            }else{
                Wheel_01_flag = Wheel_02_flag = Wheel_03_flag = Wheel_04_flag = Door_L1_flag = Door_L2_flag = Door_R1_flag = Door_R2_flag = Door_B_flag = 0;
                meshes.push_back(processMesh(mesh, scene));
            }
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
            //cout << "node num = " << ++num << endl;
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // Walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                printf("str.C_Str() = %s\n", str.C_Str());
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1){
            format = GL_RED;
        }
        else if (nrComponents == 3){
            format = GL_RGB;
        }
        else if (nrComponents == 4){
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif
