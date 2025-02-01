#include <iostream>

#include <vector>
#include "LightSystem.h"
//#include "Core/Components/PointLight.h"
#include "Core/Components/DirectionalLight.h"

#include "Core/Scene/SceneManager.h"
#include "Core/Systems/TransformSystem.h"

using namespace AE::Graphics;
using namespace AE::Core;

void LightSystem::Initialize() {
	
	//allocate the buffer to zero
	uint32 bufferSize = sizeof(PointLightGPU) * MAX_LIGHTS ;

	std::vector<uint32> initalBuffer(bufferSize, 0);

	lightCountLocation = sizeof(PointLight) * MAX_LIGHTS;

	lightBuffer = GraphicsManager::GetInstance().CreateBuffer(
		initalBuffer.data(),
		MAX_LIGHTS,
		0,
		BufferType::Uniform
	);

	lightBuffer->Bind(3);

	SceneManager& sceneManager = SceneManager::GetInstance();
	auto& registry = sceneManager.Registry;

	dirLight = registry.create();

	registry.emplace<DirectionalLight>(dirLight, Vec3{ 0, -1, 0 }, Vec3{1, 1, 1});
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

		if (lightMap.contains(&pointLight)) {
			UpdateLightGPU(lightMap[&pointLight], pointLight, pointLightTransform);
		}
		else {
			AddLightGPU(pointLight, pointLightTransform);
		}
		i++;
	}

	//process dirlight(can be done in parallel with above function)
	//set rasterizer
	//update render targets
	//gather all shadow behaving objects, and then render them
	
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
	//lightBuffer->Update2(&lightCount, lightCountLocation, sizeof(unsigned int));
}