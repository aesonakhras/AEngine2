#include "ResourceManager.h"

using namespace AE::Core;
using namespace AE::Graphics;

//must be removed by 12/15/2024
#include "Graphics/CommonUBOs.h"

#include "System/Input/InputManager.h"


bool ResourceManager::initialize() {
	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::R,
		AE::System::InputState::Pressed,
		std::bind(&ResourceManager::recompileAllShaders, this)
	);

	return true;
}

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

std::shared_ptr<AE::Graphics::Material> ResourceManager::LoadMaterial(std::string vertexShaderName, std::string fragmentShaderName, std::string id) {
	
	if (MaterialBaseCache.GetItem(id) != nullptr) {
		auto sharedMaterial = MaterialInstanceCache[id][0];

		if (sharedMaterial == nullptr) {
			//error
		}

		return sharedMaterial;
	}

	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	StandardUniformBuffer mvp;
	mvp.mWorldViewProj = DirectX::XMMatrixIdentity();

	std::vector<AE::Graphics::UniformDescription> uniformDescription = {
		{"MVP", sizeof(DirectX::XMMATRIX)},
		{"Model", sizeof(DirectX::XMMATRIX)},
		{"ViewDir", sizeof(DirectX::XMVECTOR)},
		{"DirLight", sizeof(DirectX::XMVECTOR)}
	};

	auto materialBase = GetMaterialBase(id, vertexShaderName, fragmentShaderName, uniformDescription);

	auto material = graphicsManager.CreateMaterialInstance(
		materialBase,
		&mvp,
		sizeof(StandardUniformBuffer)
	);


	//initialize the list
	MaterialInstanceCache[id] = {};

	MaterialInstanceCache[id].push_back(material);

	return MaterialInstanceCache[id][0];
}

std::shared_ptr<AE::Graphics::Material> ResourceManager::CreateMaterialInstance(std::string materialID) {
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();
	
	if (MaterialBaseCache.GetItem(materialID) == nullptr) {
		//error
	}

	auto materialBase = MaterialBaseCache.GetItem(materialID);

	StandardUniformBuffer mvp;
	mvp.mWorldViewProj = DirectX::XMMatrixIdentity();

	auto material = graphicsManager.CreateMaterialInstance(
		materialBase,
		&mvp,
		sizeof(StandardUniformBuffer)
	);

	MaterialInstanceCache[materialID] = {};

	MaterialInstanceCache[materialID].push_back(material);

	return material;

}

std::shared_ptr<AE::Graphics::Material> ResourceManager::GetSharedMaterial(std::string materialID) {
	if (MaterialBaseCache.GetItem(materialID) == nullptr) {
		//error
	}

	return MaterialInstanceCache[materialID][0];
}

std::shared_ptr<AE::Graphics::IVertexShader> ResourceManager::GetVertexShader(std::string shaderName) {
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	auto vertexShader = graphicsManager.CreateVertexShader(
		shaderName,
		AE::Graphics::StandardVertexDescription::Get()
	);

	if (vertexShader != nullptr) {
		//add to the cache
		vertexShaderCache.addItem(shaderName, vertexShader, 0, false);
	}
	else {
		//uh oh
	}

	return vertexShader;
}

std::shared_ptr<AE::Graphics::IFragmentShader> ResourceManager::GetFragmentShader(std::string shaderName) {
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	auto fragmentShader = graphicsManager.CreateFragmentShader(shaderName);

	if (fragmentShader != nullptr) {
		fragmentShaderCache.addItem(shaderName, fragmentShader, 0, false);
	}
	else {
		//uh oh
	}

	return fragmentShader;
}

std::shared_ptr<AE::Graphics::MaterialBase> ResourceManager::GetMaterialBase(
	std::string materialName,
	std::string vertexShaderName,
	std::string fragmentShaderName,
	std::vector<AE::Graphics::UniformDescription> uniformDescription
) {
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	auto vertexshader = GetVertexShader(vertexShaderName);
	auto fragmentShader = GetFragmentShader(fragmentShaderName);

	auto materialBase = graphicsManager.CreateMaterialBase(vertexshader, fragmentShader, uniformDescription);

	if (materialBase != nullptr) {
		MaterialBaseCache.addItem(materialName, materialBase, 0, false);
	}
	else {
		//uh oh
	}

	return materialBase;
}

void ResourceManager::recompileAllShaders() {
	for (auto& [key, item] : vertexShaderCache) {
		recompileVertexShader(key, item.resource);
	}

	for (auto& [key, item] : fragmentShaderCache) {
		recompileFragmentShader(key, item.resource);
	}
}

void ResourceManager::recompileVertexShader(std::string name, std::shared_ptr<AE::Graphics::IVertexShader> vertexShader) {
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	graphicsManager.RecompileVertexShader(name, vertexShader);
}

void ResourceManager::recompileFragmentShader(std::string name, std::shared_ptr<AE::Graphics::IFragmentShader> fragmentShader) {
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	graphicsManager.RecompileFragmentShader(name, fragmentShader);
}

void ResourceManager::shutdown() { return; }
