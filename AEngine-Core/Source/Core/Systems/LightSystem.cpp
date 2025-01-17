#include <iostream>

#include <vector>
#include "LightSystem.h"
//#include "Core/Components/PointLight.h"

#include "Core/Scene/SceneManager.h"
#include "Core/Systems/TransformSystem.h"

using namespace AE::Graphics;
using namespace AE::Core;

void LightSystem::Initialize() {
	
	//allocate the buffer to zero
	uint32 bufferSize = sizeof(PointLightGPU) * MAX_LIGHTS;

	std::vector<uint32> initalBuffer(bufferSize, 0);

	lightCountLocation = sizeof(PointLight) * MAX_LIGHTS;

	lightBuffer = GraphicsManager::GetInstance().CreateBuffer(
		initalBuffer.data(),
		MAX_LIGHTS,
		0,
		BufferType::Uniform
	);

	lightBuffer->Bind(3);
}

void LightSystem::Update() {
	SceneManager& sceneManager = SceneManager::GetInstance();
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	auto& registry = sceneManager.Registry;

	graphicsManager.StartFrame();

	//before this grab the camera
	auto updatedLightsView = sceneManager.Registry.view<PointLight, Transform, TransformUpdatedTag>();

	int i = 0;
	for (auto& light : updatedLightsView) {
		auto& pointLight = registry.get<PointLight>(light);
		auto& pointLightTransform = registry.get<Transform>(light);

		if (lightMap.contains(&pointLight)) {
			UpdateLightGPU(lightMap[&pointLight], pointLight, pointLightTransform);
		}
		else {
			AddLightGPU(pointLight, pointLightTransform);
		}
		i++;
	}
}

void LightSystem::AddLightGPU(AE::Graphics::PointLight& pointlight, Transform& transform) {
	uint32 insertIndex = nextAvailableLight++;

	lightMap[&pointlight] =  insertIndex;

	UpdateLightGPU(insertIndex, pointlight, transform);
}

void LightSystem::UpdateLightGPU(uint32 index, AE::Graphics::PointLight& pointlight, Transform& transform) {
	Vec3 Position = transform.GetWorldPosition();

	DirectX::XMVECTOR dxPosition = { Position.X, Position.Y, Position.Z, 1.0f };

	PointLightGPU gpuLight = {
		dxPosition,
		{pointlight.Color.X, pointlight.Color.Y, pointlight.Color.Z, 0.0f},
		pointlight.Radius,
		pointlight.Linear,
		pointlight.Quad,
		0.0f
	};

	assert((sizeof(PointLightGPU) * index) % 16 == 0 && "GPU buffer update must be 16-byte aligned");


	lightCount++;

	lightBuffer->Update2(&gpuLight, sizeof(PointLightGPU) * index, sizeof(PointLightGPU));
}