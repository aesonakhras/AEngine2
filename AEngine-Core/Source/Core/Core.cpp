

#include <memory.h>
#include <string>
#include <vector>

//AEngine Specific

#include "Core.h"
#include "Core/Common.h"

#include "Graphics/GraphicsManager.h"

#include "Graphics/DeviceCreateInfo.h"
#include "Graphics/TextureCreateInfo.h"
#include "Graphics/CommonVerticies.h"

#include "FileManagment/FileManager.h"
#include "FileManagment/FileImporter.h"

#include "System/Window/WindowFactory.h"

#include "System/Time/DeltaTimeManager.h"
#include "System/Input/InputManager.h"
#include "System/Audio/AudioManager.h"
#include "System/Memory/MemoryTracker.h"

#include "Core/Systems/SystemLocator.h"
#include "Core/Systems/TransformSystem.h"
#include "Core/Systems/RigidBodySystem.h"
#include "Core/Systems/RenderSystem.h"
#include "Core/Systems/LightSystem.h"
#include "Core/Scene/SceneManager.h"

#include "Physics/PhysicsManager.h"

#include "Resources/ResourceManager.h"

#define WINDOW_START_X 300
#define WINDOW_START_Y 300

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

using namespace AE::Core;
using namespace AE::Graphics;
using namespace AE::Physics;

RefCountPtr<AE::System::IWindow> window;

float32 deltaTime = 0.0f;

std::function<void(float32, JobSystem&, CommandBuffer&)> appUpdate;

RenderSystem renderSystem;
LightSystem lightSystem;

AE::Core::TransformSystem transformSystem;
RigidBodySystem rigidBodySystem;

entt::registry* g_sceneRegistry;

JobSystem jobSystem{8};

CommandBuffer commandBuffer {};

float32 physicsTimeStep = 1.0f / 144.0f;

float32 timeSinceLastPhysicsStep = 0.0f;

void AE::Core::Run() {
    //force delta time to zero for the first frame
    AE::System::DeltaTimeManager::GetInstance().StartFrame();
    AE::System::DeltaTimeManager::GetInstance().LimitFrameRate();

    while (!window->GetShouldEngineExit()) {
        AE::System::DeltaTimeManager::GetInstance().StartFrame();
        deltaTime = AE::System::DeltaTimeManager::GetInstance().GetDeltaTime();

        timeSinceLastPhysicsStep += deltaTime;

        AE::System::InputManager::GetInstance().Update();
        appUpdate(deltaTime, jobSystem, commandBuffer);

        //execute jobs
        jobSystem.WaitForCompletion();

        transformSystem.Update();

        //execute command buffer for shared variable data in order
        commandBuffer.Execute();

        while(timeSinceLastPhysicsStep >= physicsTimeStep) {
            PhysicsManager::GetInstance().StepPhysics(timeSinceLastPhysicsStep, physicsTimeStep);

            rigidBodySystem.Update(
                SceneManager::GetInstance().Registry,
                deltaTime
            );
            timeSinceLastPhysicsStep -= physicsTimeStep;
        }

        GraphicsManager::GetInstance().StartFrame();

        lightSystem.Update();
        renderSystem.Render();

        transformSystem.ClearUpdated();
        SceneManager::GetInstance().RemoveDeletedEntities();
        AE::System::DeltaTimeManager::GetInstance().LimitFrameRate();
    }
}

void AE::Core::Start(std::function<void(float32, JobSystem&, CommandBuffer&)> cb) {
    appUpdate = cb;

    AE::System::FileManager::Initialize();

    std::string windowName = "AEngine";

    AE::System::WindowCreateInfo windowCreateInfo {
        WINDOW_START_X,
        WINDOW_START_Y,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        windowName
    };

    window = AE::System::WindowFactory::Create(windowCreateInfo);

    AE::System::InputManager::Initialize(window);

    AE::Graphics::DeviceCreateInfo createInfo { SCREEN_WIDTH, SCREEN_HEIGHT, *window};

    AE::Graphics::GraphicsManager::Initialize(createInfo);

    AE::Core::ResourceManager::Initialize();

    AE::System::AudioManager::Initialize();

    AE::System::DeltaTimeManager::Initialize(144);

    AE::Physics::PhysicsManager::Initialize();

    AE::Core::SceneManager::Initialize();

    transformSystem.SetScene(SceneManager::GetInstance().Registry);

    AE::Core::SystemLocator::Register<AE::Core::TransformSystem>(&transformSystem);

    lightSystem.Initialize();
}


void AE::Core::ShutDown() {
    AE::Core::SceneManager::ShutDown();
    AE::System::FileManager::ShutDown();
    AE::System::InputManager::ShutDown();
    AE::Core::ResourceManager::ShutDown();
    AE::Graphics::GraphicsManager::ShutDown();
    AE::System::AudioManager::ShutDown();
    AE::System::DeltaTimeManager::ShutDown();

    AE::System::MemoryAllocator::DonaldDUMP();
}
