#include "PlayerSystem.h"

#include "Components/Player.h"
#include "Components/Movement.h"

#include "Core/Components/Transform.h"
#include "System/Input/InputManager.h"


using namespace AE::Core;
using namespace AE::App;

void PlayerSystem::Update(float32 deltaTime,
	entt::registry& scene,
	AE::Core::JobSystem& jobSystem,
	AE::Core::CommandBuffer& commandBuffer
) {
	jobSystem.SubmitJob([deltaTime, &scene, &commandBuffer]() {
	
		auto playerView = scene.view<Player, Transform, Movement>();

		if (playerView.begin() == playerView.end()) {
			return;
		}

		auto playerEntity = playerView.front();

		auto& player = playerView.get<Player>(playerEntity);
		auto& playerTransform = playerView.get<Transform>(playerEntity);
		auto& playerMovement = playerView.get<Movement>(playerEntity);

		DirectX::XMVECTOR deltaRotation = DirectX::XMQuaternionRotationAxis({ 0, 1, 0 }, deltaTime);

		Vec3 normalizedDir = playerMovement.Dir.Normalized();

		Vec3 newDeltaMovement = normalizedDir * (deltaTime * player.movementSpeed);

		commandBuffer.Submit([&playerTransform, deltaRotation, newDeltaMovement]() {
			//add a small rotation to the player
			//playerTransform.SetRotation(DirectX::XMQuaternionMultiply(playerTransform.GetRotation(), deltaRotation));
			playerTransform.SetPosition(playerTransform.GetPosition() + newDeltaMovement);
		});
	});
	

}

void PlayerSystem::Start(entt::registry& scene) {
	m_scene = &scene;

	auto playerView = m_scene->view<Movement>();

	if (playerView.begin() == playerView.end()) {
		return;
	}

	auto playerEntity = playerView.front();
	playerMovementCache = &playerView.get<Movement>(playerEntity);

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

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::Q,
		AE::System::InputState::Pressed,
		std::bind(&PlayerSystem::OnQDown, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::Q,
		AE::System::InputState::Released,
		std::bind(&PlayerSystem::OnQUp, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::Z,
		AE::System::InputState::Pressed,
		std::bind(&PlayerSystem::OnZDown, this)
	);

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::Z,
		AE::System::InputState::Released,
		std::bind(&PlayerSystem::OnZUp, this)
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

void PlayerSystem::OnQDown() {
	modifyMovement(0.0f, 1.0f, 0.0f);
}

void PlayerSystem::OnQUp() {
	modifyMovement(0.0f, -1.0f, 0.0f);
}

void PlayerSystem::OnZDown() {
	modifyMovement(0.0f, -1.0f, 0.0f);
}

void PlayerSystem::OnZUp() {
	modifyMovement(0.0f, 1.0f, 0.0f);
}

inline void PlayerSystem::modifyMovement(float x, float y, float z) {
	playerMovementCache->Dir += {x, y, z};
}
