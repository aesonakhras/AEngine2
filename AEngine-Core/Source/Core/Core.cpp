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

#include "../Texture.h"

#include "../FileImporter.h"

#include "../DeviceCreateInfo.h"

#include "../FileManagment/FileManager.h"

#include "../Window/WindowFactory.h"

#include "../Graphics/TextureCreateInfo.h"

#include "../Graphics/IFragmentShader.h"

#include "../Graphics/CommonVerticies.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

namespace Core {
    AE::Graphics::GraphicsManager g_GraphicsManager;

    std::shared_ptr<AE::Graphics::IVertexShader> vertexShader = nullptr;
    std::shared_ptr<AE::Graphics::IFragmentShader> fragmentShader = nullptr;

    std::vector<StaticMesh*> meshes;

    std::shared_ptr<Texture> texture;

    Camera camera = { {0.0f, 0.0f, -1.0f, 0.0f}, 1.0472f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.01f, 100.0f };

    std::unique_ptr<AE::Core::System::IWindow> window;

    void ImportMesh(std::string fileName) {
        MeshData meshData = FileImporter::ImportMesh(fileName);

        auto vertexBuffer = g_GraphicsManager.CreateBuffer((void*)meshData.vertexData, meshData.vertexCount, sizeof(AE::Graphics::StandardVertex), AE::Graphics::BufferType::Vertex);
        auto indexBuffer = g_GraphicsManager.CreateBuffer((void*)meshData.indexData, meshData.indexCount, sizeof(unsigned int), AE::Graphics::BufferType::Index);

        Transform transform{
            {0.0f, 0.0f, 1.0f, 0.0f}, //pos
            {0.0f, 0.0f, 0.0f, 0.0f}, //rot
            {1.0f, 1.0f, 1.0f}  //scale
        };

        meshes.push_back(new StaticMesh{ vertexBuffer, indexBuffer, vertexShader, fragmentShader, transform});
    }

    void textureSetup() {
        AE::Graphics::TextureCreateInfo textureData = FileImporter::ImportTexture(std::string("CatTP.png"));
        
        textureData.depth = 1;
        textureData.mipLevels = 1;
        textureData.bindFlags = AE::Graphics::ShaderResource;
        textureData.generateMipMaps = false;
        textureData.arraySize = 1;
        textureData.sampleCount = 1;
        
        texture = g_GraphicsManager.CreateTexture(textureData);

        texture->Bind();
    }

    void SetupScene() {
        vertexShader = g_GraphicsManager.CreateVertexShader("shaders.shader", AE::Graphics::StandardVertexDescription::Get());

        fragmentShader = g_GraphicsManager.CreateFragmentShader("shaders.shader");

        Transform transform{
            {0.0f, 0.0f, 1.0f, 0.0f}, //pos
            {0.0f, 0.0f, 0.0f, 0.0f}, //rot
            {1.0f, 1.0f, 1.0f}  //scale
        };

        Transform transform2 {
            {10.0f, 10.0f, 100.0f, 0.0f}, //pos
            {0.0f, 0.0f, 0.0f, 0.0f}, //rot
            {1.5f, 1.5f, 1.5f}  //scale
        };

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

        int i = 0;
        for (auto mesh : meshes) {
            if (i == 0) {
                Transform transform{
                    {0.0f, -1.0f, zPos, 1.0f}, //pos
                    {1.5f, y, 0.0f, 0.0f}, //rot
                    {0.025f, 0.025f, 0.025f, 1.0f}  //scale
                };

                mesh->SetTransform(transform);
            }
            i++;
        }
        texture->Bind();
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