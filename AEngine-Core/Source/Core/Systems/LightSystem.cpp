#include <iostream>

#include <vector>
#include "LightSystem.h"
//#include "Core/Components/PointLight.h"
#include "Core/Components/DirectionalLight.h"
#include "Core/Components/Camera.h"

#include "Core/Scene/SceneManager.h"
#include "Core/Systems/TransformSystem.h"

#include "Core/Components/SkyBox.h"


//TODO: Move to a smarter place
#include "Core/Events/EventManager.h"


using namespace AE::Graphics;
using namespace AE::Core;
using namespace DirectX;

void LightSystem::Initialize() {
	//set up light removal code


	//AE::System::InputManager::GetInstance().RegisterButtonEvent(
	//	AE::System::Button::W,
	//	AE::System::InputState::Pressed,
	//	std::bind(&PlayerSystem::OnForwardDown, this)
	//);

	EventManager::Subscribe<EntityDestroyedEvent>(
		std::bind(
			&LightSystem::RemoveLight,
			this,
			std::placeholders::_1
		)
	);

	//allocate the buffer to zero
	uint32 bufferSize = sizeof(PointLightGPU) * MAX_LIGHTS ;

	std::vector<uint32> initalBuffer(bufferSize, 0);

	lightBuffer = GraphicsManager::GetInstance().CreateBuffer(
		initalBuffer.data(),
		bufferSize,
		0,
		BufferType::Uniform
	);

	uint32 lightCount = 0;

	lightCountBuffer = GraphicsManager::GetInstance().CreateBuffer(
		&lightCount,
		sizeof(uint32) * 4,
		0,
		BufferType::Uniform
	);

	lightCountBuffer->Bind(2);
	lightBuffer->Bind(3);

	SceneManager& sceneManager = SceneManager::GetInstance();
	auto& registry = sceneManager.Registry;

	dirLight = registry.create();

	registry.emplace<DirectionalLight>(dirLight, Vec3{ 0, -1, 0 }, Vec3{1, 1, 1});
	
	Transform transform {
		dirLight,
		{0, 20, 0},
		DirectX::XMQuaternionRotationAxis(
			DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XM_PIDIV2 + 0.45f 
		),
		{ 1.0f, 1.0f, 1.0f },
		nullptr,
		"DirLight"
	};

	registry.emplace<Transform>(dirLight, transform);

	registry.emplace<AE::Core::Camera>(
		dirLight,
		0,
		0,
		0.01,
		100,
		true,
		50
	);

	auto camera = registry.get<Camera>(dirLight);

	//to see where the dir light is pointing
	//SceneManager::GetInstance().mainCameraEntity = dirLight;
}

void LightSystem::Update() {
	SceneManager& sceneManager = SceneManager::GetInstance();

	auto& registry = sceneManager.Registry;

	//before this grab the camera
	auto updatedLightsView = sceneManager.Registry.view<PointLight, Transform, TransformUpdatedTag>();

	int i = 0;
	for (auto& light : updatedLightsView) {
		auto& pointLight = registry.get<PointLight>(light);
		auto& pointLightTransform = registry.get<Transform>(light);

		if (lightMap.contains(light)) {
			UpdateLightGPU(lightMap[light]);
		}
		else {
			AddLightGPU(light, pointLight, pointLightTransform);
		}
		i++;
	}

	
	handleDirLightPass();
}

void LightSystem::AddLightGPU(
	entt::entity entity,
	AE::Graphics::PointLight& pointlight,
	Transform& transform
) {
	int index = lightArray.Add(PointLightData{ &pointlight, &transform, entity });

	//insert into the map
	lightMap[entity] = index;

	//mark index dirty
	dirtyLights.Set(index);

	UpdateLightGPU(index);

	uint32 size = lightArray.Size();

	lightCountBuffer->Update2(&size, 0, sizeof(uint32));
}

void LightSystem::UpdateLightGPU(uint32 index) {
	auto& data = lightArray.Get(index);

	Vec3 worldPosition = data.transform->GetWorldPosition();
	
	PointLight* pointLight = data.pointLight;

	pointLight->Color.X;

	DirectX::XMVECTOR dxPosition = { worldPosition.X, worldPosition.Y, worldPosition.Z, 1.0f};

	PointLightGPU gpuLight = {
		dxPosition,
		{
			pointLight->Color.X,
			pointLight->Color.Y,
			pointLight->Color.Z,
			0.0f
		},
		pointLight->Radius,
		pointLight->Linear,
		pointLight->Quad,
		0.0f
	};

	lightBuffer->Update2(&gpuLight, sizeof(PointLightGPU) * index, sizeof(PointLightGPU));
}

void LightSystem::RemoveLight(const EntityDestroyedEvent& event) {
	//check if entity is in the map

	uint32 pointLightIndex = -1;

	if (lightMap.contains(event.entity)) {
		uint32 pointLightIndex = lightMap[event.entity];

		lightArray.Remove(pointLightIndex);

		lightMap[lightArray.Get(pointLightIndex).entity] = pointLightIndex;

		//the data at pointLightIndex is now dirty
		UpdateLightGPU(pointLightIndex);

		uint32 size = lightArray.Size();
		lightCountBuffer->Update2(&size, 0, sizeof(uint32));
	}
}

void LightSystem::handleDirLightPass() {
	//process dirlight(can be done in parallel with above function)
	//set rasterizer
	//update render targets

	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	graphicsManager.SetRasterState("Shadow");
	SceneManager& sceneManager = SceneManager::GetInstance();
	auto& registry = sceneManager.Registry;

	auto& directionalLight = registry.get<DirectionalLight>(dirLight);
	auto& directionalLightCamera = registry.get<Camera>(dirLight);
	auto& directionalLightTransform = registry.get<Transform>(dirLight);


	directionalLight.ViewPort->Bind();
	

	auto renderablesView = registry.view<Mesh, Transform>(entt::exclude<SkyBox>);

	auto projection = directionalLightCamera.ProjectionMatrix;
	auto view = DirectX::XMMatrixInverse(nullptr, directionalLightTransform.WorldMatrix);

	auto uhhhh = directionalLightTransform.GetForwardDir();

	auto vp = view * projection;

	directionalLight.DepthTexture->UnBind(10);
	
	directionalLight.DepthTexture->ClearRenderTarget();
	directionalLight.DepthTexture->BindAsRenderTarget(0);

	//gather all shadow behaving objects, and then render them
	for (const auto& renderable : renderablesView) {
		auto& mesh = registry.get<Mesh>(renderable);
		auto& transform = registry.get<Transform>(renderable);

		DirectX::XMMATRIX finalMatrix = transform.WorldMatrix * vp;
		
		directionalLight.DirLightMaterial->SetUniform("lightViewProj", finalMatrix);
		directionalLight.DirLightMaterial->Bind();

		mesh.VertexBuffer->Bind(0);
		mesh.IndexBuffer->Bind(0);

		graphicsManager.Draw(mesh.IndexBuffer->Count);
	}

	directionalLight.DepthTexture->UnBindRenderTarget(0);
	directionalLight.DepthTexture->Bind(10);

	graphicsManager.SetRasterState("Default");
}