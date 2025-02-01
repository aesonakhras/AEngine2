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

		entt::entity dirLight;

		std::string shadow = R"(
			struct VS_Input
			{
				float3 position : POSITION;
				float3 normal   : NORMAL;
				float3 tangent  : TANGENT;
				float3 bitangent : BITANGENT;
				float2 uv       : TEXCOORD;
			};

			struct VS_Output
			{
				float4 posLightSpace : SV_POSITION;
			};

			cbuffer LightMatrices : register(b0)
			{
				matrix lightViewProj;
			}

			VS_Output VS_Shadow(VS_Input input)
			{
				VS_Output output;
				output.posLightSpace = mul(lightViewProj, float4(input.position, 1.0f));
				return output;
			}

			void PS_Shadow(VS_Output input)
			{
				//depth only
			}
		)";
	};
}