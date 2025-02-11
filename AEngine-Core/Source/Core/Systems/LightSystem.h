#pragma once
#include <memory>
#include <DirectXMath.h>
#include <unordered_map>

#include "Graphics/GraphicsManager.h"
#include "Core/Components/PointLight.h"
#include "Core/Components/Transform.h"

#include "Core/Containers/DirtyFlagArray.h"
#include "Core/Containers/PackedArray.h"

#include "Core/Events/DestroyEntityEvent.h"

#include "Core/Types.h"

#define MAX_LIGHTS 128

namespace AE::Graphics {
	class LightSystem {
	public:

		//TODO: Probably Dependency injection
		void Initialize();

		void Update();
	private:
		//decleration of GPU side light
		struct PointLightGPU {
			DirectX::XMVECTOR position;
			DirectX::XMVECTOR color;
			float32 radius;
			float32 linearAttenuation;
			float32 quadraticAttenuation;
			float32 padding;
		};

		struct PointLightData
		{
			PointLight* pointLight;
			Transform* transform;
			entt::entity entity;
		};

		std::shared_ptr<AE::Graphics::IBuffer> lightBuffer;
		std::shared_ptr<AE::Graphics::IBuffer> lightCountBuffer;

		uint32 nextAvailableLight = 0;
		std::unordered_map<entt::entity, uint32> lightMap{};

		uint32 lightCountLocation = 0;

		uint32 lightCount = 0;

		void AddLightGPU(
			entt::entity entity,
			AE::Graphics::PointLight& pointlight, 
			Transform& transform
		);

		void UpdateLightGPU(uint32 index);

		void RemoveLight(const EntityDestroyedEvent& event);

		void handleDirLightPass();
		entt::entity dirLight;


		DirtyFlagArray<MAX_LIGHTS> dirtyLights;
		PackedArray<PointLightData, MAX_LIGHTS> lightArray;

	};
}