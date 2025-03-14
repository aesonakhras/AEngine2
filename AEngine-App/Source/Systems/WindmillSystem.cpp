#include <DirectXMath.h>
#include <iostream>
#include "WindmillSystem.h"

#include "Components/Windmill.h"

#include "Core/Components/Transform.h"

#include "Core/Systems/SystemLocator.h"
#include "Core/Systems/TransformSystem.h"
#include "Core/Components/RigidBody.h"

#include "Core/Scene/SceneManager.h"

#include "Components/Player.h"
#include "Factories/ProjectileFactory.h"

#include "System/Audio/AudioManager.h"

using namespace AE::Core;
using namespace AE::App;
using namespace AE::Physics;

static int counter = 0;

void WindmillSystem::Start(entt::registry& scene) {
	//register with the on collision
	
	this->scene = &scene;
	//get all windmills, and register their cb's accordingly

	auto windmillView = scene.view<WindMill, Transform, RigidBody>();

	for (auto entity : windmillView) {
		auto& rb = scene.get<RigidBody>(entity);
		rb.OnCollision = std::bind(&WindmillSystem::OnCollision, this, std::placeholders::_1, entity);
	}
}

void WindmillSystem::Update(float32 deltaTime,
	entt::registry& scene,
	AE::Core::JobSystem& jobSystem,
	AE::Core::CommandBuffer& commandBuffer
	) {
	static float initialWait = 10.0f;

	auto windmillView = scene.view<WindMill, Transform>();

	if (windmillView.begin() == windmillView.end()) {
		return;
	}

	for (auto windmillEntity : windmillView) {
		jobSystem.SubmitJob([deltaTime, &scene, &commandBuffer, &windmillView, windmillEntity, this]() {
			auto* windMill = scene.try_get<WindMill>(windmillEntity);

			entt::entity pinEntity = windMill->pin;

			auto* windMillTransform = scene.try_get<Transform>(pinEntity);

			auto transforSystem = SystemLocator::Get<TransformSystem>();

			DirectX::XMVECTOR deltaRot = DirectX::XMQuaternionRotationAxis({ 1, 0, 0 }, windMill->rotationSpeed * deltaTime);

			DirectX::XMVECTOR newRot = DirectX::XMQuaternionMultiply(transforSystem->GetLocalRotation(pinEntity), deltaRot);

			transforSystem->SetLocalRotation(pinEntity, newRot);

			initialWait -= deltaTime;

			if (initialWait >= 0) return;

			windMill->TimeSinceShot += deltaTime;
			if (windMill->TimeSinceShot >= fireRate) {
				auto playerView = scene.view<Player, Transform>();

				if (playerView.begin() == playerView.end()) {
					return;
				}

				entt::entity playerEntity = *playerView.begin();

				auto* playerTransform = scene.try_get<Transform>(playerEntity);

				if (!playerTransform) return;

				Vec3 windMillPos = windMillTransform->GetWorldPosition();
				Vec3 playerPos = playerTransform->GetWorldPosition();

				Vec3 dir = playerPos - windMillPos;
				dir = dir.Normalized();

				//TODO: Remove magic number LMAO
				Vec3 spawnPoint = dir * 8.0f;

				ProjectileFactory::Create(
					dir,
					windMillPos + spawnPoint,
					30,
					"_w"
				);

				AE::System::AudioManager::GetInstance().PlayAudio("gunshot2");

				std::cout << "FIRE!" << std::endl;
				windMill->TimeSinceShot = 0.0f;
			}
			
		});
	}

	counter++;
}

void WindmillSystem::OnCollision(const AE::Physics::OnCollisionInfo& collisionInfo, entt::entity self) {
	
	static int lastCollisionCount = 0;

	if (counter == lastCollisionCount) {
		return;
	}

	//determine if this is ignore or not
	Transform* otherTransform =  scene->try_get<Transform>(collisionInfo.Other);

	if (otherTransform == nullptr || otherTransform->Name.find("_w") != std::string::npos) return;

	

	lastCollisionCount = counter;
	WindMill& windmill = scene->get<WindMill>(self);

	//this is on the EDGE of a refactor lmao,
	//not my usual code but need to move on
	if (windmill.health == 4) {
		AE::Core::SceneManager::GetInstance().DeleteEntity(windmill.rotor1);
	}
	else if (windmill.health == 3) {
		AE::Core::SceneManager::GetInstance().DeleteEntity(windmill.rotor2);
	}
	else if (windmill.health == 2)
	{
		AE::Core::SceneManager::GetInstance().DeleteEntity(windmill.rotor3);
	}
	else if (windmill.health == 1) {
		AE::Core::SceneManager::GetInstance().DeleteEntity(windmill.rotor4);
	}
	else if (windmill.health == 0) {
		AE::Core::SceneManager::GetInstance().DeleteEntity(self);
	}

	//bad system
	AE::Core::SceneManager::GetInstance().DeleteEntity(collisionInfo.Other);
	AE::System::AudioManager::GetInstance().PlayAudio("hit");
	
	windmill.health--;
	
}