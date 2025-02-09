#pragma once
#include <memory>

#include "Math/Vec3.h"
#include "Graphics/Texture.h"

#include "Graphics/GraphicsManager.h"
#include "Core/Debug.h"
#include "Resources/ResourceManager.h"

namespace AE::Core {
	struct DirectionalLight
	{
		Vec3 Dir;
		Vec3 Color;
		std::unique_ptr<AE::Graphics::Texture> DepthTexture;
		std::shared_ptr<AE::Graphics::Material> DirLightMaterial;

		std::shared_ptr<AE::Graphics::IViewport> ViewPort;

		DirectionalLight(Vec3 dir, Vec3 color) : Dir(dir), Color(color) {
			AE::Graphics::TextureCreateInfo info{};

			info.height = 2048;
			info.width = 2048;
			info.data = nullptr;
			info.generateMipMaps = false;
			info.type = AE::Graphics::TextureType::Texture2D;

			//directional light will default to 32bit precision
			info.format = AE::Graphics::TextureFormat::R32_TYPELESS;
			info.use = AE::Graphics::TextureUse::DepthTexture;

			DepthTexture = AE::Graphics::GraphicsManager::GetInstance().CreateTexture(info);

			if (!DepthTexture) {
				Debug::Log("Unable to create Dir light depth texture.");
			}

			//create the material
			std::vector<AE::Graphics::UniformDescriptionElement> PlayeruniformDescription = {
				{"lightViewProj", sizeof(DirectX::XMMATRIX)},
			};

			DirLightMaterial = ResourceManager::GetInstance().LoadMaterial(dirLightShaderName, dirLightShaderName, "DirLightShader", PlayeruniformDescription);
			
			AE::Graphics::ViewPortCreateInfo viewportInfo{};
			viewportInfo.height = 2048;
			viewportInfo.width = 2048;
			viewportInfo.x = 0;
			viewportInfo.y = 0;
			viewportInfo.minDepth = 0;
			viewportInfo.maxDepth = 1;
			
			ViewPort = AE::Graphics::GraphicsManager::GetInstance().CreateViewPort(viewportInfo);
		}

	private:
		const std::string dirLightShaderName = "Assets/DirLightShader.shader";
	};
}