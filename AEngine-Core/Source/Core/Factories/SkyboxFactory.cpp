#include "SkyboxFactory.h"
#include "Core/Components/SkyBox.h"
#include "Resources/ResourceManager.h"

using namespace AE::Core;
using namespace AE::Graphics;

entt::entity SkyboxFactory::Create(
	entt::registry& registry,
	std::string Visual,
	std::string Radiance,
	std::string Irradiance
) {
	//set up the material
	GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

	//load the mesh
	std::shared_ptr<Mesh> skyBoxMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/skybox.obj"));
	

	std::shared_ptr<Texture> VisualTexture = ResourceManager::GetInstance().GetTexture(Visual, true, false);
	std::shared_ptr<Texture> RadianceTexture = ResourceManager::GetInstance().GetTexture(Radiance, true, false);
	std::shared_ptr<Texture> IrradianceTexture = ResourceManager::GetInstance().GetTexture(Irradiance, true, false);

	std::shared_ptr<AE::Graphics::ISampler> sampler = graphicsManager.CreateSampler(false);


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

	//haha why is 9 afraid of 7?? ahaha
	skyBoxMaterial->SetTexture("sdfgsdrgdrger", 7, VisualTexture, sampler);
	skyBoxMaterial->SetTexture("skyboxRadiance", 8, RadianceTexture, sampler);
	skyBoxMaterial->SetTexture("skyboxIrradiance", 9, IrradianceTexture, sampler);

	entt::entity skybox = registry.create();

	//add the skybox component for fun lmao
	registry.emplace<Mesh>(skybox, *skyBoxMesh.get());
	registry.emplace<Material>(skybox, *skyBoxMaterial.get());
	registry.emplace<SkyBox>(skybox);

	
	return skybox;
}
