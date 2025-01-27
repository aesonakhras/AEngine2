#include <iostream>
#include <string>
#include <memory>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ktx.h"

#include "FileImporter.h"
#include "Core/Types.h"

using namespace AE::Graphics;


TextureCreateInfo FileImporter::ImportTexture(std::string textureName, bool isCubeMap) {
    TextureCreateInfo textureData{};

    //check if file is .ktx
    if (textureName.ends_with(".ktx2")) {
        ImportKTXFile(textureName, textureData);

        return textureData;
    }

    if (isCubeMap) {
        ImportCubeMap(textureName, textureData);
    }
    else {
        ImportTexture2D(textureName, textureData);
    }

    return textureData;
}

void FileImporter::ImportTexture2D(const std::string& fileName, TextureCreateInfo& textureCreateInfo) {
    uint32 channels = 0;

    void* img = static_cast<void*> (
        ImportImageFile(
            fileName,
            textureCreateInfo.width,
            textureCreateInfo.height,
            channels
        )
    );

    if (img == nullptr) {
        std::cout << "image failed to load" << std::endl;
    }

    textureCreateInfo.data = std::unique_ptr<uint8[]>(static_cast<uint8_t*>(img));
    textureCreateInfo.dataSize = textureCreateInfo.width * textureCreateInfo.height * channels;
    textureCreateInfo.format = DetermineTextureFormat(channels);
    textureCreateInfo.type = AE::Graphics::TextureType::Texture2D;
    textureCreateInfo.arraySize = 1;
}

//Convention for import will be fileName = folderName/ <- make sure to put slash
//then rely on naming convention in cubeMapNames
void FileImporter::ImportCubeMap(const std::string& fileName, TextureCreateInfo& textureCreateInfo) {
    static const int CUBE_MAP_SIDES = 6;

    //in correct order for looping, and use by directx
    static const std::vector<std::string> cubeMapNames {
        "right",
        "left",
        "top",
        "bottom",
        "front",
        "back"
    };

    uint32 channels = 0;

    
    for (int i = 0; i < CUBE_MAP_SIDES; i++) {
        //TODO: Get rid of hardcoding eventually
        std::string finalFileName = fileName + cubeMapNames[i] + ".png";

        void* img = static_cast<void*> (
            ImportImageFile(
                finalFileName,
                textureCreateInfo.width,
                textureCreateInfo.height,
                channels
            )
        );

        if (img != nullptr) {
            textureCreateInfo.data.push_back(img);
        }
    }

    //we end up using the width, height, and channels from the last loaded image
    //They need to be same width and textures
    if (textureCreateInfo.data.size() != CUBE_MAP_SIDES) {
        std::cout << "ERROR IN LOADING CUBE MAP" << std::endl;
    }

    textureCreateInfo.format = DetermineTextureFormat(channels);
    textureCreateInfo.type = AE::Graphics::TextureType::Cubemap;
    textureCreateInfo.arraySize = CUBE_MAP_SIDES;
}

unsigned char* FileImporter::ImportImageFile(const std::string& fileName, uint32& width, uint32& height, uint32& channels) {
    unsigned char* img = stbi_load(
        fileName.c_str(),
        (int*)&width,
        (int*)&height,
        (int*)&channels,
        0
    );

    //TODO: Some error checking
    return img;

}

TextureFormat FileImporter::DetermineTextureFormat(uint32 channels) {
    switch (channels)
    {
        case(1):
            return TextureFormat::R8U;
        case(2):
            std::cout << "no support yet" << std::endl;
            return TextureFormat::NOFORMAT;
        case(3):
            std::cout << "no support yet" << std::endl;
            return TextureFormat::NOFORMAT;
        case(4):
            return TextureFormat::RGBA8U;
        default:
            break;
    }

    return TextureFormat::NOFORMAT;
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


//TODO: This function needs double the amount of memory then it really should
//Due to copying over the ktxTexture managed data to our managed data.
//I want to keep this abstraction
//Could benefit from a new library later potentially
void FileImporter::ImportKTXFile(const std::string& filename, AE::Graphics::TextureCreateInfo& textureCreateInfo) {
    ktxTexture* texture = nullptr;

    // Load the KTX file
    KTX_error_code result = ktxTexture_CreateFromNamedFile(
        filename.c_str(),
        KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
        &texture
    );

    if (result != KTX_SUCCESS) {
        //Todo assert
    }

    std::cout << "Successfully loaded KTX file: " << filename << std::endl;
    std::cout << "Texture dimensions: " << texture->baseWidth << "x" << texture->baseHeight << std::endl;
    std::cout << "Number of mip levels: " << texture->numLevels << std::endl;
    std::cout << "Depth" << texture->baseDepth << std::endl;
    
    textureCreateInfo.width = texture->baseWidth;
    textureCreateInfo.height = texture->baseHeight;
    textureCreateInfo.depth = texture->baseDepth;
    textureCreateInfo.mipLevels = texture->numLevels;
    textureCreateInfo.format = DetermineTextureFormat(4);
    textureCreateInfo.type = AE::Graphics::TextureType::Texture2D;
    textureCreateInfo.arraySize = 1;
    textureCreateInfo.dataSize = texture->dataSize;

    //copy data
    textureCreateInfo.data = std::make_unique<uint8[]>(textureCreateInfo.dataSize);
    memcpy(textureCreateInfo.data.get(), ktxTexture_GetData(texture), textureCreateInfo.dataSize);

    ktxTexture_Destroy(texture);
}
