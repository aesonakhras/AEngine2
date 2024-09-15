#include "Core.h"
#include "../GraphicsManager.h"

//I want to see most of the below #include GONE
//for fun
#include <memory.h>
#include <string>
#include <vector>

//AEngine Specific
#include "../StaticMesh.h"
#include "../Camera.h"

#include "../Graphics/Texture.h"

#include "../FileImporter.h"

#include "../DeviceCreateInfo.h"

#include "../FileManagment/FileManager.h"

#include "../Window/WindowFactory.h"

#include "../Graphics/TextureCreateInfo.h"

#include "../Graphics/CommonVerticies.h"

#include "../Core/WorldObject.h"
#include "../Graphics/ISampler.h" 

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

namespace Core {
    struct MVP_ONLY_BUFFER
    {
        DirectX::XMMATRIX mWorldViewProj;
    };

    AE::Graphics::GraphicsManager g_GraphicsManager;

    std::vector<StaticMesh*> meshes;

    Camera camera = { {0.0f, 0.0f, -1.0f, 0.0f}, 1.0472f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.01f, 1000.0f };

    std::unique_ptr<AE::Core::System::IWindow> window;

    std::shared_ptr < AE::Graphics::Material> material;

    void ImportMesh(std::string fileName) {
        MeshData meshData = FileImporter::ImportMesh(fileName);

        auto vertexBuffer = g_GraphicsManager.CreateBuffer((void*)meshData.vertexData, meshData.vertexCount, sizeof(AE::Graphics::StandardVertex), AE::Graphics::BufferType::Vertex);
        auto indexBuffer = g_GraphicsManager.CreateBuffer((void*)meshData.indexData, meshData.indexCount, sizeof(unsigned int), AE::Graphics::BufferType::Index);

        std::shared_ptr<AE::Core::WorldObject> worldObj = std::make_shared<AE::Core::WorldObject>();

        meshes.push_back(new StaticMesh{ vertexBuffer, indexBuffer, material, worldObj });
    }

    void textureSetup() {
        AE::Graphics::TextureCreateInfo textureData = FileImporter::ImportTexture(std::string("part1.png"));
        
        textureData.depth = 1;
        textureData.mipLevels = 1;
        textureData.bindFlags = AE::Graphics::ShaderResource;
        textureData.generateMipMaps = false;
        textureData.arraySize = 1;
        textureData.sampleCount = 1;
        
        std::shared_ptr<AE::Graphics::Texture> texture1 = g_GraphicsManager.CreateTexture(textureData);

        AE::Graphics::TextureCreateInfo textureData2 = FileImporter::ImportTexture(std::string("part2.png"));

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
            "shaders.shader", 
            AE::Graphics::StandardVertexDescription::Get(),
            &mvp,
            sizeof(MVP_ONLY_BUFFER)
        );

        textureSetup();
        ImportMesh(std::string("Cat.obj"));
    }

    //oooh not very good, this is quite stinky
    //but for now we will pass it references to the things that we need
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

    void Start() {
        AE::Core::System::FileManager& fileManager = AE::Core::System::FileManager::GetInstance();

        std::string windowName = "AEngine";

        AE::Core::System::WindowCreateInfo windowCreateInfo{ SCREEN_WIDTH, SCREEN_HEIGHT, windowName };

        window = AE::Core::System::WindowFactory::Create(windowCreateInfo);

        AECore::DeviceCreateInfo createInfo { SCREEN_HEIGHT, SCREEN_WIDTH, *window};

        ///////////////////////////////////Set Up Direct X related stuff////////////////////////////////////
        g_GraphicsManager.Initialize(createInfo);
        SetupScene();
        Simulate();

        g_GraphicsManager.ShutDown();
    }
}