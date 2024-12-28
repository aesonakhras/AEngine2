#include "PlayerSystem.h"

#include "Components/Player.h"
#include "Components/Movement.h"

#include "Core/Components/Transform.h"
#include "System/Input/InputManager.h"


using namespace AE::Core;
using namespace AE::App;

void PlayerSystem::Update(AE::Core::float32 deltaTime,
	entt::registry& scene,
	AE::Core::JobSystem& jobSystem,
	AE::Core::CommandBuffer& commandBuffer
) {
	jobSystem.SubmitJob([deltaTime, &scene, &commandBuffer]() {
	
		auto playerView = scene.view<Player, Transform, Movement>();

		auto playerEntity = playerView.front();

		auto& player = playerView.get<Player>(playerEntity);
		auto& playerTransform = playerView.get<Transform>(playerEntity);
		auto& playerMovement = playerView.get<Movement>(playerEntity);

		DirectX::XMVECTOR deltaRotation = DirectX::XMQuaternionRotationAxis({ 0, 1, 0 }, deltaTime);

		DirectX::XMVECTOR normalizedDir = DirectX::XMVector4Normalize(playerMovement.Dir);

		DirectX::XMVECTOR newDeltaMovement = DirectX::XMVectorScale(normalizedDir, deltaTime * player.movementSpeed);

		commandBuffer.Submit([&playerTransform, deltaRotation, newDeltaMovement]() {
			//add a small rotation to the player
			playerTransform.Rotation = DirectX::XMQuaternionMultiply(playerTransform.Rotation, deltaRotation);
			playerTransform.Position = DirectX::XMVectorAdd(playerTransform.Position, newDeltaMovement);
		});
	});
	

}

void PlayerSystem::Start(entt::registry& scene) {
	m_scene = &scene;

	auto playerview = m_scene->view<Movement>();
	auto playerEntity = playerview.front();
	playerMovementCache = &playerview.get<Movement>(playerEntity);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::W,
		AE::System::InputState::Pressed,
		std::bind(&PlayerSystem::OnForwardDown, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::W,
		AE::System::InputState::Released,
		std::bind(&PlayerSystem::OnForwardUp, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::A,
		AE::System::InputState::Pressed,
		std::bind(&PlayerSystem::OnLeftDown, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::A,
		AE::System::InputState::Released,
		std::bind(&PlayerSystem::OnLeftUp, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::S,
		AE::System::InputState::Pressed,
		std::bind(&PlayerSystem::OnBackwardDown, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::S,
		AE::System::InputState::Released,
		std::bind(&PlayerSystem::OnBackwardUp, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::D,
		AE::System::InputState::Pressed,
		std::bind(&PlayerSystem::OnRightDown, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::D,
		AE::System::InputState::Released,
		std::bind(&PlayerSystem::OnRightUp, this)
	);
}

void PlayerSystem::OnForwardDown() {
	modifyMovement(0.0f, 0.0f , 1.0f);
}

void PlayerSystem::OnForwardUp() {
	modifyMovement(0.0f, 0.0f, -1.0f);
}

void PlayerSystem::OnBackwardDown() {
	modifyMovement(0.0f, 0.0f, -1.0f);
}

void PlayerSystem::OnBackwardUp() {
	modifyMovement(0.0f, 0.0f, 1.0f);
}

void PlayerSystem::OnRightDown() {
	modifyMovement(1.0f, 0.0f, 0.0f);
}

void PlayerSystem::OnRightUp() {
	modifyMovement(-1.0f, 0.0f, 0.0f);
}

void PlayerSystem::OnLeftDown() {
	modifyMovement(-1.0f, 0.0f, 0.0f);
}

void PlayerSystem::OnLeftUp() {
	modifyMovement(1.0f, 0.0f, 0.0f);
}

inline void PlayerSystem::modifyMovement(float x, float y, float z) {
	playerMovementCache->Dir = DirectX::XMVectorAdd(
		playerMovementCache->Dir,
		DirectX::XMVectorSet(x, y, z, 0.0f)
	);
}
