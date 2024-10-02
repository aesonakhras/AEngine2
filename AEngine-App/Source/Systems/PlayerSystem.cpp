#include "PlayerSystem.h"

#include "Components/Player.h"
#include "Core/Components/Transform.h"


using namespace AE::Core;
using namespace AE::App;

void PlayerSystem::Update(AE::Core::float32 deltaTime,
	entt::registry& scene,
	AE::Core::JobSystem& jobSystem,
	AE::Core::CommandBuffer& commandBuffer
) {
	jobSystem.SubmitJob([deltaTime, &scene, &commandBuffer]() {
	
		auto playerView = scene.view<Player, Transform>();

		auto playerEntity = playerView.front();

		auto& player = playerView.get<Player>(playerEntity);
		auto& playerTransform = playerView.get<Transform>(playerEntity);

		DirectX::XMVECTOR deltaRotation = DirectX::XMQuaternionRotationAxis({ 0, 1, 0 }, deltaTime);

		commandBuffer.Submit([&playerTransform, deltaRotation]() {
			//add a small rotation to the player
			playerTransform.Rotation = DirectX::XMQuaternionMultiply(playerTransform.Rotation, deltaRotation);

		});
	});
	

}