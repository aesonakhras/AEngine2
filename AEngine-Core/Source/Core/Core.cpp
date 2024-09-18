

#include <memory.h>
#include <string>
#include <vector>

//AEngine Specific

#include "Core.h"
#include "Core/WorldObject.h"
#include "Core/Common.h"

#include "Graphics/ISampler.h" 
#include "Graphics/GraphicsManager.h"
#include "Graphics/StaticMesh.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/DeviceCreateInfo.h"
#include "Graphics/TextureCreateInfo.h"
#include "Graphics/CommonVerticies.h"

#include "FileManagment/FileManager.h"
#include "FileManagment/FileImporter.h"

#include "System/Window/WindowFactory.h"

#define WINDOW_START_X 300
#define WINDOW_START_Y 300

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

using namespace AE::Core;

struct MVP_ONLY_BUFFER
{
    DirectX::XMMATRIX mWorldViewProj;
};

AE::Graphics::GraphicsManager g_GraphicsManager;

std::vector<StaticMesh*> meshes;

Camera camera = { {0.0f, 0.0f, -1.0f, 0.0f}, 1.0472f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.01f, 1000.0f };

UniquePtr<AE::System::IWindow> window;

RefCountPtr<AE::Graphics::Material> material;

void ImportMesh(std::string fileName) {
    MeshData meshData = FileImporter::ImportMesh(fileName);

    auto vertexBuffer = g_GraphicsManager.CreateBuffer((void*)meshData.vertexData, meshData.vertexCount, sizeof(AE::Graphics::StandardVertex), AE::Graphics::BufferType::Vertex);
    auto indexBuffer = g_GraphicsManager.CreateBuffer((void*)meshData.indexData, meshData.indexCount, sizeof(unsigned int), AE::Graphics::BufferType::Index);

    RefCountPtr<AE::Core::WorldObject> worldObj = MakeRef<AE::Core::WorldObject>();

    meshes.push_back(new StaticMesh{ vertexBuffer, indexBuffer, material, worldObj });
}

void textureSetup() {
    AE::Graphics::TextureCreateInfo textureData = FileImporter::ImportTexture(std::string("Assets/part1.png"));
        
    textureData.depth = 1;
    textureData.mipLevels = 1;
    textureData.bindFlags = AE::Graphics::ShaderResource;
    textureData.generateMipMaps = false;
    textureData.arraySize = 1;
    textureData.sampleCount = 1;
        
    std::shared_ptr<AE::Graphics::Texture> texture1 = g_GraphicsManager.CreateTexture(textureData);

    AE::Graphics::TextureCreateInfo textureData2 = FileImporter::ImportTexture(std::string("Assets/part2.png"));

    textureData2.depth = 1;
    textureData2.mipLevels = 1;
    textureData2.bindFlags = AE::Graphics::ShaderResource;
    textureData2.generateMipMaps = false;
    textureData2.arraySize = 1;
    textureData2.sampleCount = 1;

    std::shared_ptr<AE::Graphics::Texture> texture2 = g_GraphicsManager.CreateTexture(textureData2);

    std::shared_ptr<AE::Graphics::ISampler> sampler = g_GraphicsManager.CreateSampler();

    material->SetTexture("diffuse1", 0, texture1, sampler);
    material->SetTexture("diffuse2", 1, texture2, sampler);
}

void SetupScene() {
    MVP_ONLY_BUFFER mvp;
    mvp.mWorldViewProj =  DirectX::XMMatrixIdentity();

    material = g_GraphicsManager.CreateMaterial(
        "Assets/shaders.shader", 
        AE::Graphics::StandardVertexDescription::Get(),
        &mvp,
        sizeof(MVP_ONLY_BUFFER)
    );

    textureSetup();
    ImportMesh(std::string("Assets/Cat.obj"));
}

void renderFrame() {
    static float y = 0;
    float zPos = 1;
    y += 0.0001f;
    if (y > 6.28) {
        y = 0.0f;
    }
    // y = 5.0f;
    auto mesh = meshes[0];

    mesh->m_worldObject->SetPosition({ 0 , -1.0f, 100, 1.0f });
    mesh->m_worldObject->SetRotation({ 1.5f, y, 0.0f, 0.0f });

    g_GraphicsManager.DrawFrame(meshes, camera.GetVP());
}

void Simulate() {
    while (!window->ShouldEngineExit()) {
        renderFrame();
    }
}

void AE::Core::Start() {
    AE::System::FileManager& fileManager = AE::System::FileManager::GetInstance();

    std::string windowName = "AEngine";

    AE::System::WindowCreateInfo windowCreateInfo{
        WINDOW_START_X,
        WINDOW_START_Y,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        windowName
    };

    window = AE::System::WindowFactory::Create(windowCreateInfo);

    AE::Graphics::DeviceCreateInfo createInfo { SCREEN_HEIGHT, SCREEN_WIDTH, *window};

    ///////////////////////////////////Set Up Direct X related stuff////////////////////////////////////
    g_GraphicsManager.Initialize(createInfo);
    SetupScene();
    Simulate();

    g_GraphicsManager.ShutDown();
}
