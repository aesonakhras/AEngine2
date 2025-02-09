#pragma once
#include <memory>
#include <DirectXMath.h>
#include <unordered_map>

#include "Graphics/GraphicsManager.h"
#include "Core/Components/PointLight.h"
#include "Core/Components/Transform.h"
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

		std::shared_ptr<AE::Graphics::IBuffer> lightBuffer;

		uint32 nextAvailableLight = 0;
		std::unordered_map<PointLight*, uint32> lightMap{};

		uint32 lightCountLocation = 0;

		uint32 lightCount = 0;

		void AddLightGPU(AE::Graphics::PointLight& pointlight, Transform& transform);

		void UpdateLightGPU(uint32 index, AE::Graphics::PointLight& pointlight, Transform& transform);

		void handleDirLightPass();
		entt::entity dirLight;
	};
}