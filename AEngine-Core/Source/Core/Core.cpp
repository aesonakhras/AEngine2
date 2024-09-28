

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

#include "Core/Systems/RenderSystem.h"
#include "Core/Scene/SceneManager.h"

#define WINDOW_START_X 300
#define WINDOW_START_Y 300

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

using namespace AE::Core;

RefCountPtr<AE::System::IWindow> window;

float32 deltaTime = 0.0f;

std::function<void(float32)> appUpdate;

RenderSystem renderSystem;

entt::registry* g_sceneRegistry;

void AE::Core::Run() {
    while (!window->GetShouldEngineExit()) {
        AE::System::DeltaTimeManager::GetInstance().StartFrame();
        deltaTime = AE::System::DeltaTimeManager::GetInstance().GetDeltaTime();

        AE::System::InputManager::GetInstance().Update();
        appUpdate(deltaTime);

        //here
        renderSystem.Render();

        AE::System::DeltaTimeManager::GetInstance().LimitFrameRate();
    }
}

void AE::Core::Start(std::function<void(float32)> cb) {
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

    AE::Graphics::DeviceCreateInfo createInfo { SCREEN_HEIGHT, SCREEN_WIDTH, *window};

    AE::Graphics::GraphicsManager::Initialize(createInfo);

    AE::System::AudioManager::Initialize();

    AE::System::DeltaTimeManager::Initialize(144);

    AE::Core::SceneManager::Initialize();
}


void AE::Core::ShutDown() {
    AE::Core::SceneManager::ShutDown();
    AE::System::FileManager::ShutDown();
    AE::System::InputManager::ShutDown();
    AE::Graphics::GraphicsManager::ShutDown();
    AE::System::AudioManager::ShutDown();
    AE::System::DeltaTimeManager::ShutDown();
}
