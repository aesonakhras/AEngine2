#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Utils/Singleton.h"

#include "Graphics/Texture.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/CommonVerticies.h"
#include "Graphics/Mesh.h"
#include "Core/Components/Material.h"

#include "Graphics/IBuffer.h"

#include "FileManagment/FileImporter.h"

#include "ResourceCache.h"

#include "Core/Debug.h"


namespace AE::Core {
	class ResourceManager : public AE::Utils::Singleton<ResourceManager> {
		public:
			std::shared_ptr<AE::Graphics::Texture> GetTexture(std::string id);
			std::shared_ptr<AE::Graphics::Mesh> GetStaticMesh(std::string id);
			std::shared_ptr<AE::Graphics::Material> GetMaterial(std::string vertexShaderName, std::string fragmentShaderName, std::string id);
			//material

		protected:
			friend class AE::Utils::Singleton<ResourceManager>;

			bool initialize();
			void shutdown();

		private:
			std::shared_ptr<AE::Graphics::IVertexShader> GetVertexShader(std::string shaderName);
			std::shared_ptr<AE::Graphics::IFragmentShader> GetFragmentShader(std::string shaderName);
			std::shared_ptr<AE::Graphics::MaterialBase> GetMaterialBase(
				std::string materialName,
				std::string vertexShaderName,
				std::string fragmentShaderName,
				std::vector<AE::Graphics::UniformDescription> uniformDescription
			);

			ResourceCache<AE::Graphics::Texture> textureCache;
			ResourceCache<AE::Graphics::Mesh> meshCache;
			ResourceCache<AE::Graphics::Material> materialCache;
			ResourceCache<AE::Graphics::MaterialBase> MaterialBaseCache;

			ResourceCache<AE::Graphics::IFragmentShader> fragmentShaderCache;
			ResourceCache<AE::Graphics::IVertexShader> vertexShaderCache;
	};
}