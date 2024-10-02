#include <iostream>
#include <string>
#include <memory>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "FileImporter.h"

AE::Graphics::TextureCreateInfo FileImporter::ImportTexture(std::string textureName) {
    AE::Graphics::TextureCreateInfo textureData{};
    
    const char* filename = textureName.c_str();
    int channels= 0;

     unsigned char* img = stbi_load(filename,
                                (int*)&textureData.width, (int*)&textureData.height, &channels, 0);

     textureData.data = (void*)img;

    if (img == nullptr) {
        std::cout << "image failed to load" << std::endl;
    }

    switch (channels)
    {
        case(1):
            textureData.format = TextureFormat::R8U;
        case(2):
            std::cout << "no support" << std::endl;
            //textureData.format = R8B8;
        case(3):
            std::cout << "no support" << std::endl;
            //textureData.format = R8B8G8;
        case(4):
            textureData.format = TextureFormat::RGBA8U;
        default:
            break;
    }

    return textureData;
}

MeshData FileImporter::ImportMesh(std::string fileName) {
    static Assimp::Importer importer;

    MeshData meshData{};
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(fileName,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_MakeLeftHanded |
        aiProcess_FlipWindingOrder |
        aiProcess_FlipUVs);

    // If the import failed, report it
    if (nullptr == scene) {
        std::cout << "model has failed" << std::endl;
    }
    if (scene->HasMeshes()) {
        std::cout << "model exists, parsing now" << std::endl;
    }

    size_t indiciesCount = scene->mMeshes[0]->mNumFaces * 3;
    unsigned int* indicies = new unsigned int[indiciesCount];
    //std::vector<unsigned int> indicies{};


    int lazyCounter = 0;
    for (int i = 0; i < scene->mMeshes[0]->mNumFaces; i++) {
        for (int j = 0; j < 3; j++) {
            indicies[lazyCounter++] = scene->mMeshes[0]->mFaces[i].mIndices[j];
        }
    }

    //assume user wants vertex, normal, tangent, bitangent, and uv so 8
    size_t size = scene->mMeshes[0]->mNumVertices * 14;

    float* vertexData = new float[size];

    lazyCounter = 0;
    for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++) {
        auto vertex = scene->mMeshes[0]->mVertices[i];
        auto normal = scene->mMeshes[0]->mNormals[i];
        auto tanget = scene->mMeshes[0]->mTangents[i];
        auto bitangent = scene->mMeshes[0]->mBitangents[i];
        auto uv = scene->mMeshes[0]->mTextureCoords[0][i];

        vertexData[lazyCounter++] = vertex.x;
        vertexData[lazyCounter++] = vertex.y;
        vertexData[lazyCounter++] = vertex.z;

        vertexData[lazyCounter++] = normal.x;
        vertexData[lazyCounter++] = normal.y;
        vertexData[lazyCounter++] = normal.z;

        vertexData[lazyCounter++] = tanget.x;
        vertexData[lazyCounter++] = tanget.y;
        vertexData[lazyCounter++] = tanget.z;

        vertexData[lazyCounter++] = bitangent.x;
        vertexData[lazyCounter++] = bitangent.y;
        vertexData[lazyCounter++] = bitangent.z;

        vertexData[lazyCounter++] = uv.x;
        vertexData[lazyCounter++] = uv.y;
    }
    meshData.vertexData = (void*)vertexData;
    meshData.indexData = (void*)indicies;

    meshData.indexCount = indiciesCount;
    meshData.vertexCount = scene->mMeshes[0]->mNumVertices;

    return meshData;
}