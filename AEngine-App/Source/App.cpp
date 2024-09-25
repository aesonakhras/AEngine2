#include <memory>
#include <vector>

#include "Core/Core.h"

//TODO: determine if this should just be in a common.h or core.h or something
#include "Graphics/GraphicsManager.h"
#include "Graphics/StaticMesh.h"
#include "Graphics/Material.h"
#include "Graphics/CommonVerticies.h"
#include "Graphics/Camera.h"

#include "FileManagment/MeshData.h"
#include "FileManagment/FileImporter.h"

#include "System/Audio/AudioManager.h"
#include "System/Input/InputManager.h"

#include <iostream>

using namespace AE::Core;
using namespace AE::Graphics;

struct MVP_ONLY_BUFFER
{
    DirectX::XMMATRIX mWorldViewProj;
};

std::shared_ptr<Material> material;

std::vector<std::shared_ptr<StaticMesh>> meshes;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Camera camera = { {0.0f, 0.0f, -1.0f, 0.0f}, 1.0472f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 1000.0f };

void ImportMesh(std::string fileName) {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    MeshData meshData = FileImporter::ImportMesh(fileName);

    auto vertexBuffer = graphicsManager.CreateBuffer(
        (void*)meshData.vertexData,
        meshData.vertexCount,
        sizeof(AE::Graphics::StandardVertex),
        AE::Graphics::BufferType::Vertex
    );

    auto indexBuffer = graphicsManager.CreateBuffer(
        (void*)meshData.indexData,
        meshData.indexCount,
        sizeof(unsigned int),
        AE::Graphics::BufferType::Index
    );

    RefCountPtr<AE::Core::WorldObject> worldObj = MakeRef<AE::Core::WorldObject>();

    meshes.push_back(std::make_shared<StaticMesh>( vertexBuffer, indexBuffer, material, worldObj ));
}

void textureSetup() {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    AE::Graphics::TextureCreateInfo textureData = FileImporter::ImportTexture(std::string("Assets/part1.png"));

    textureData.depth = 1;
    textureData.mipLevels = 1;
    textureData.bindFlags = AE::Graphics::ShaderResource;
    textureData.generateMipMaps = false;
    textureData.arraySize = 1;
    textureData.sampleCount = 1;

    std::shared_ptr<AE::Graphics::Texture> texture1 = graphicsManager.CreateTexture(textureData);

    AE::Graphics::TextureCreateInfo textureData2 = FileImporter::ImportTexture(std::string("Assets/part2.png"));

    textureData2.depth = 1;
    textureData2.mipLevels = 1;
    textureData2.bindFlags = AE::Graphics::ShaderResource;
    textureData2.generateMipMaps = false;
    textureData2.arraySize = 1;
    textureData2.sampleCount = 1;

    std::shared_ptr<AE::Graphics::Texture> texture2 = graphicsManager.CreateTexture(textureData2);

    std::shared_ptr<AE::Graphics::ISampler> sampler = graphicsManager.CreateSampler();

    material->SetTexture("diffuse1", 0, texture1, sampler);
    material->SetTexture("diffuse2", 1, texture2, sampler);
}

void SetupScene() {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    MVP_ONLY_BUFFER mvp;
    mvp.mWorldViewProj = DirectX::XMMatrixIdentity();

    material = graphicsManager.CreateMaterial(
        "Assets/shaders.shader",
        AE::Graphics::StandardVertexDescription::Get(),
        &mvp,
        sizeof(MVP_ONLY_BUFFER)
    );

    textureSetup();
    ImportMesh(std::string("Assets/Cat.obj"));
}

void OnPressed() {
    AE::System::AudioManager::GetInstance().PlayAudio("gunshot");
}

void Update(float32 deltaTime) {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    static float32 y = 0;
    float32 zPos = 1;
    y += 1.0f * deltaTime;
    if (y > 6.28) {
        y = 0.0f;
    }
    // y = 5.0f;
    auto mesh = meshes[0];

    mesh->m_worldObject->SetPosition({ 0 , -1.0f, 100, 1.0f });
    mesh->m_worldObject->SetRotation({ 1.5f, y, 0.0f, 0.0f });

    graphicsManager.DrawFrame(meshes, camera.GetVP());
}

//startup
void StartApp() {
    SetupScene();

    AE::System::InputManager::GetInstance().RegisterButtonEvent(AE::System::Button::W, AE::System::InputState::Pressed, OnPressed);

    AE::System::AudioManager::GetInstance().LoadAudioClip("Assets/Sound/gunshot.wav", "gunshot");
}

//update <- for now before updateable is implemented

int main()
{		
    AE::Core::Start(Update);

	StartApp();

	AE::Core::Run();

	AE::Core::ShutDown();
	
	return 0;
}

