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

//TODO: Split the resource manager into seperate things,
//I do not like that I have pulled in this
#include "Physics/PhysicsManager.h"
#include "Physics/Bounds.h"


namespace AE::Core {
	class ResourceManager : public AE::Utils::Singleton<ResourceManager> {
		public:
			std::shared_ptr<AE::Graphics::Texture> GetTexture(std::string id, bool isCubeMap, bool generateMipMaps);
			std::shared_ptr<AE::Graphics::Mesh> GetStaticMesh(std::string id);
			
			std::shared_ptr<AE::Graphics::Material> LoadMaterial(
				std::string vertexShaderName,
				std::string fragmentShaderName,
				std::string id,
				const std::vector<AE::Graphics::UniformDescriptionElement>& uniformDescriptionElements
			);

			std::shared_ptr<AE::Graphics::Material> CreateMaterialInstance(std::string materialID);

			std::shared_ptr<AE::Graphics::Material> GetSharedMaterial(std::string materialID);
			std::shared_ptr<btConvexHullShape> GetConvexHull(std::string id);

		protected:
			friend class AE::Utils::Singleton<ResourceManager>;

			bool initialize();
			void shutdown();

		private:
			void recompileAllShaders();

			void recompileVertexShader(std::string name, std::shared_ptr<AE::Graphics::IVertexShader> vertexShader);
			void recompileFragmentShader(std::string name, std::shared_ptr<AE::Graphics::IFragmentShader> fragmentShader);

			std::shared_ptr<AE::Graphics::IVertexShader> GetVertexShader(std::string shaderName);
			std::shared_ptr<AE::Graphics::IFragmentShader> GetFragmentShader(std::string shaderName);
			std::shared_ptr<AE::Graphics::MaterialBase> GetMaterialBase(
				std::string materialName,
				std::string vertexShaderName,
				std::string fragmentShaderName,
				AE::Graphics::UniformLayoutDescription& uniformDescription
			);

			

			ResourceCache<AE::Graphics::Texture> textureCache;
			ResourceCache<AE::Graphics::Mesh> meshCache;

			std::unordered_map<std::string, std::vector<std::shared_ptr<AE::Graphics::Material>>> MaterialInstanceCache;

			ResourceCache<AE::Graphics::MaterialBase> MaterialBaseCache;

			ResourceCache<AE::Graphics::IFragmentShader> fragmentShaderCache;
			ResourceCache<AE::Graphics::IVertexShader> vertexShaderCache;
			ResourceCache<btConvexHullShape> convexHulls;
	};
}