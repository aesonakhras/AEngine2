#include <DirectXMath.h>
#include <iostream>
#include "WindmillSystem.h"

#include "Components/Windmill.h"

#include "Core/Components/Transform.h"

#include "Core/Systems/SystemLocator.h"
#include "Core/Systems/TransformSystem.h"
#include "Core/Components/RigidBody.h"

#include "Core/Scene/SceneManager.h"

using namespace AE::Core;
using namespace AE::App;
using namespace AE::Physics;

void WindmillSystem::Start(entt::registry& scene, entt::entity self) {
	//register with the on collision
	auto& rb = scene.get<RigidBody>(self);

	rb.OnCollision = std::bind(&WindmillSystem::OnCollision, this, std::placeholders::_1);

}

void WindmillSystem::Update(float32 deltaTime,
	entt::registry& scene,
	AE::Core::JobSystem& jobSystem,
	AE::Core::CommandBuffer& commandBuffer
	) {
	jobSystem.SubmitJob([deltaTime, &scene, &commandBuffer]() {

		auto windmillView = scene.view<WindMill, Transform>();

		if (windmillView.begin() == windmillView.end()) {
			return;
		}

		auto playerEntity = windmillView.front();

		auto& player = windmillView.get<WindMill>(playerEntity);
		auto& playerTransform = windmillView.get<Transform>(playerEntity);

		auto transforSystem = SystemLocator::Get<TransformSystem>();

		DirectX::XMVECTOR deltaRot = DirectX::XMQuaternionRotationAxis({ 1, 0, 0 }, player.rotationSpeed * deltaTime);

		DirectX::XMVECTOR newRot = DirectX::XMQuaternionMultiply(transforSystem->GetLocalRotation(playerEntity), deltaRot);

		transforSystem->SetLocalRotation(playerEntity, newRot);
	});


}

void WindmillSystem::OnCollision(const AE::Physics::OnCollisionInfo& collisionInfo) {
	AE::Core::SceneManager::GetInstance().DeleteEntity(collisionInfo.Other);
}