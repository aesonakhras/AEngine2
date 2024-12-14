#include "ResourceManager.h"

using namespace AE::Core;
using namespace AE::Graphics;

std::shared_ptr<AE::Graphics::Texture> ResourceManager::GetTexture(std::string id) {
	//check to see if resource exists
	auto resource = textureCache.GetItem(id);

	if (resource) {
		return resource;
	}
	else {
		//if not load it and add it to the resource cache
		GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

		AE::Graphics::TextureCreateInfo textureData = FileImporter::ImportTexture(id);

		textureData.depth = 1;
		textureData.mipLevels = 1;
		textureData.bindFlags = AE::Graphics::ShaderResource;
		textureData.generateMipMaps = false;
		textureData.arraySize = 1;
		textureData.sampleCount = 1;

		auto texture = graphicsManager.CreateTextureUnsafe(textureData);

		bool result = textureCache.addItem(
			id,
			std::make_unique<Texture>(texture),
			textureData.arraySize,
			false
		);

		if (result) {
			return textureCache.GetItem(id);
		}
		else {
			return nullptr;
		}
	}
}

std::shared_ptr<AE::Graphics::Mesh> ResourceManager::GetStaticMesh(std::string id) {
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();
	MeshData meshData = FileImporter::ImportMesh(std::string(id));

	//buffers
	auto VB = graphicsManager.CreateBuffer(
		meshData.vertexData,
		meshData.vertexCount,
		sizeof(AE::Graphics::StandardVertex),
		AE::Graphics::BufferType::Vertex
	);

	auto IB = graphicsManager.CreateBuffer(
		meshData.indexData,
		meshData.indexCount,
		sizeof(unsigned int),
		AE::Graphics::BufferType::Index
	);
	
	//create mesh
	auto mesh = std::make_shared<Mesh>(VB, IB);

	
	//add to cache
	bool result = meshCache.addItem(id, mesh, 0, false);

	if (result) {
		return mesh;
	}

	return nullptr;
}


bool ResourceManager::initialize() { return false; }
void ResourceManager::shutdown() { return; }
