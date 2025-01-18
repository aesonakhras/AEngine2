

#include <memory.h>
#include <string>
#include <vector>
#include <btBulletDynamicsCommon.h>

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
#include "Core/Systems/RenderSystem.h"
#include "Core/Systems/LightSystem.h"

#include "Core/Scene/SceneManager.h"

#include "Resources/ResourceManager.h"

#define WINDOW_START_X 300
#define WINDOW_START_Y 300

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

using namespace AE::Core;
using namespace AE::Graphics;

RefCountPtr<AE::System::IWindow> window;

float32 deltaTime = 0.0f;

std::function<void(float32, JobSystem&, CommandBuffer&)> appUpdate;

RenderSystem renderSystem;
LightSystem lightSystem;

AE::Core::TransformSystem transformSystem;

entt::registry* g_sceneRegistry;

JobSystem jobSystem{8};

CommandBuffer commandBuffer {};


void AE::Core::Run() {
    //force delta time to zero for the first frame
    AE::System::DeltaTimeManager::GetInstance().StartFrame();
    AE::System::DeltaTimeManager::GetInstance().LimitFrameRate();

    while (!window->GetShouldEngineExit()) {
        AE::System::DeltaTimeManager::GetInstance().StartFrame();
        deltaTime = AE::System::DeltaTimeManager::GetInstance().GetDeltaTime();

        AE::System::InputManager::GetInstance().Update();
        appUpdate(deltaTime, jobSystem, commandBuffer);

        //execute jobs
        jobSystem.WaitForCompletion();

        transformSystem.Update();

        //execute command buffer for shared variable data in order
        commandBuffer.Execute();

        lightSystem.Update();
        renderSystem.Render();

        transformSystem.ClearUpdated();
        SceneManager::GetInstance().RemoveDeletedEntities();
        AE::System::DeltaTimeManager::GetInstance().LimitFrameRate();
    }
}

void physicsTest() {
    // Step 1: Set up Bullet components
    // Collision configuration contains default setup for memory, collision setup
    btDefaultCollisionConfiguration collisionConfig;

    // Use the default collision dispatcher
    btCollisionDispatcher dispatcher(&collisionConfig);

    // Broadphase handles collision detection over large areas
    btDbvtBroadphase broadphase;

    // Default constraint solver
    btSequentialImpulseConstraintSolver solver;

    // Create the dynamics world
    btDiscreteDynamicsWorld dynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfig);

    // Set gravity in the world
    dynamicsWorld.setGravity(btVector3(0, -9.81f, 0));

    // Step 2: Create a ground plane
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape);
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

    // Add the ground to the world
    dynamicsWorld.addRigidBody(groundRigidBody);

    // Step 3: Create a dynamic sphere
    btCollisionShape* sphereShape = new btSphereShape(1);
    btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));

    int hi = 1;

    // Mass and inertia for the sphere
    btScalar sphereMass = 1;
    btVector3 sphereInertia(0, 0, 0);
    sphereShape->calculateLocalInertia(sphereMass, sphereInertia);

    btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(sphereMass, sphereMotionState, sphereShape, sphereInertia);
    btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);

    // Add the sphere to the world
    dynamicsWorld.addRigidBody(sphereRigidBody);

    // Step 4: Simulate the world
    for (int i = 0; i < 300; i++) {
        dynamicsWorld.stepSimulation(1.0f / 60.0f, 10);

        // Get and print the sphere's position
        btTransform trans;
        sphereRigidBody->getMotionState()->getWorldTransform(trans);

        std::cout << "Step " << i << ": Sphere height = " << trans.getOrigin().getY() << std::endl;
    }

    // Clean up
    dynamicsWorld.removeRigidBody(sphereRigidBody);
    delete sphereRigidBody->getMotionState();
    delete sphereRigidBody;
    delete sphereShape;

    dynamicsWorld.removeRigidBody(groundRigidBody);
    delete groundRigidBody->getMotionState();
    delete groundRigidBody;
    delete groundShape;

  
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

    AE::Core::SceneManager::Initialize();

    transformSystem.SetScene(SceneManager::GetInstance().Registry);

    AE::Core::SystemLocator::Register<AE::Core::TransformSystem>(&transformSystem);

    lightSystem.Initialize();

    physicsTest();
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
