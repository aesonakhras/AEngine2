#include "SkyboxFactory.h"
#include "Core/Components/SkyBox.h"
#include "Resources/ResourceManager.h"

using namespace AE::Core;
using namespace AE::Graphics;

entt::entity SkyboxFactory::Create(
	entt::registry& registry,
	std::string cubeMapName
) {
	//set up the material
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	//load the mesh
	std::shared_ptr<Mesh> skyBoxMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/skybox.obj"));
	std::shared_ptr<Texture> skyBoxTexture = ResourceManager::GetInstance().GetTexture(cubeMapName, true);

	std::shared_ptr<AE::Graphics::ISampler> sampler = graphicsManager.CreateSampler();


	std::vector<AE::Graphics::UniformDescriptionElement> skyboxuniformLayout = {
		{"vpNoPosition", sizeof(DirectX::XMMATRIX)},
	};

	std::shared_ptr<Material> skyBoxMaterial = ResourceManager::GetInstance().
		LoadMaterial(
			"Assets/skyBox.shader",
			"Assets/skyBox.shader",
			"SkyBoxMaterial",
			skyboxuniformLayout
		);

	skyBoxMaterial->SetTexture("skyboxTexture", 0, skyBoxTexture, sampler);

	entt::entity skybox = registry.create();

	//add the skybox component for fun lmao
	registry.emplace<Mesh>(skybox, *skyBoxMesh.get());
	registry.emplace<Material>(skybox, *skyBoxMaterial.get());
	registry.emplace<SkyBox>(skybox);

	
	return skybox;
}
