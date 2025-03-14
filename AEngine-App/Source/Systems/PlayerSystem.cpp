#include "PlayerSystem.h"

#include <DirectXMath.h>

#include "Components/Player.h"
#include "Components/Movement.h"
#include "Factories/ProjectileFactory.h"

#include "Core/Components/Transform.h"
#include "System/Input/InputManager.h"

#include "Core/Systems/SystemLocator.h"

#include "System/Audio/AudioManager.h"

using namespace AE::Core;
using namespace AE::App;
//using namespace DirectX;

#include <iostream>

float DegreesToRadians(float degrees) {
	return degrees * (3.14159 / 180);
}



void PlayerSystem::Update(float32 deltaTime,
	entt::registry& scene,
	AE::Core::JobSystem& jobSystem,
	AE::Core::CommandBuffer& commandBuffer
) {
	jobSystem.SubmitJob([this, deltaTime, &scene, &commandBuffer]() {
	
		auto playerView = scene.view<Player, Transform, Movement>();

		if (playerView.begin() == playerView.end()) {
			return;
		}

		auto playerEntity = playerView.front();

		auto& playerTransform = playerView.get<Transform>(playerEntity);
		auto& playerMovement = playerView.get<Movement>(playerEntity);
		auto& playerPlayer = playerView.get<Player>(playerEntity);
		auto transformSystem = SystemLocator::Get<TransformSystem>();

		UpdateRotation(*transformSystem, playerEntity);
		UpdateTranslation(*transformSystem, playerEntity, playerMovement, deltaTime, playerPlayer, playerTransform);
	});
}

DirectX::XMVECTOR ForwardVectorToQuaternion(const DirectX::XMVECTOR& forwardVector)
{
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX lookToMatrix = DirectX::XMMatrixLookToLH(DirectX::XMVectorZero(), DirectX::XMVector3Normalize(forwardVector), up);

	return DirectX::XMQuaternionRotationMatrix(lookToMatrix);
}

void PlayerSystem::UpdateRotation(TransformSystem& transformSystem, entt::entity playerEntity) {
	float deltaYaw = lookAxisDelta.X;
	float deltaPitch = lookAxisDelta.Y;

	// Reset look axis for the next frame
	lookAxisDelta = {};

	float sensitivity = 0.1f;

	currYaw += deltaYaw * sensitivity;
	currPitch += deltaPitch * sensitivity;

	currPitch = std::clamp(currPitch, DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(90.0f));

	float cosPitch = cosf(currPitch);
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(
		cosf(currYaw) * cosPitch,
		sinf(currPitch),
		sinf(currYaw) * cosPitch,
		0.0f
	);

	DirectX::XMVECTOR worldUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUp, forward));
	DirectX::XMVECTOR up = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(forward, right));

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMATRIX(
		right,
		up,
		forward,
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
	);

	DirectX::XMVECTOR lookQuat = DirectX::XMQuaternionRotationMatrix(rotationMatrix);
	lookQuat = DirectX::XMQuaternionNormalize(lookQuat);

	transformSystem.SetLocalRotation(playerEntity, lookQuat);
}


Vec3 fromDX(const DirectX::XMVECTOR& vector) {
	DirectX::XMFLOAT3 temp;
	DirectX::XMStoreFloat3(&temp, vector);
	return { temp.x, temp.y, temp.z };
}

void PlayerSystem::UpdateTranslation(
	AE::Core::TransformSystem& transformSystem,
	entt::entity playerEntity,
	Movement& playerMovement,
	float32 deltaTime,
	Player& player,
	Transform& transform
) {

	Vec3 normalizedDir = playerMovement.Dir.Normalized();
	 
	Vec3 newDeltaMovement = normalizedDir * (deltaTime * player.movementSpeed);

	DirectX::XMVECTOR up = DirectX::XMVector4Normalize(DirectX::XMVector4Transform(DirectX::XMVECTOR{ 0.0f, 1.0f, 0.0f, 0.0f }, transform.WorldMatrix));
	DirectX::XMVECTOR right = DirectX::XMVector4Normalize(DirectX::XMVector4Transform(DirectX::XMVECTOR{ 1.0f, 0.0f, 0.0f, 0.0f }, transform.WorldMatrix));
	DirectX::XMVECTOR forward = DirectX::XMVector4Normalize(DirectX::XMVector4Transform(DirectX::XMVECTOR{0.0f, 0.0f, 1.0f, 0.0f}, transform.WorldMatrix));

	Vec3 finalDelta = { fromDX(right) * newDeltaMovement.X + fromDX(up) * newDeltaMovement.Y + fromDX(forward) * newDeltaMovement.Z };

	Vec3 newPosition = transform.GetLocalPosition() += finalDelta;

	transformSystem.SetLocalPosition(playerEntity, newPosition);
}

void PlayerSystem::Start(entt::registry& scene) {
	m_scene = &scene;

	auto playerView = m_scene->view<Movement, Transform>();

	if (playerView.begin() == playerView.end()) {
		return;
	}

	auto playerEntity = playerView.front();

	//TODO: DO NOT CACHE THESE.  IT CAN MOVE
	playerMovementCache = &playerView.get<Movement>(playerEntity);
	playerTransformCache = &playerView.get<Transform>(playerEntity);

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

	AE::System::InputManager::GetInstance().RegisterButtonEvent(
		AE::System::Button::Space,
		AE::System::InputState::Pressed,
		std::bind(&PlayerSystem::OnSpaceDown, this)
	);

	AE::System::InputManager::GetInstance().RegisterAxisEvent(
		AE::System::AxisType::MouseX,
		std::bind(&PlayerSystem::OnHorizontal, this, std::placeholders::_1)
	);

	AE::System::InputManager::GetInstance().RegisterAxisEvent(
		AE::System::AxisType::MouseY,
		std::bind(&PlayerSystem::OnVertical, this, std::placeholders::_1)
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

void PlayerSystem::OnSpaceDown() {

	//TODO: NEVER CACHE TRANSFORMS
	Vec3 offset = Vec3(0.5f, 0.0f, 2.0f);
	DirectX::XMVECTOR offsetVec = DirectX::XMVectorSet(offset.X, offset.Y, offset.Z, 1.0f);
	DirectX::XMVECTOR offsetWorldVec = DirectX::XMVector3TransformCoord(offsetVec, playerTransformCache->WorldMatrix);

	Vec3 offsetWorld;
	DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&offsetWorld), offsetWorldVec);

	AE::App::ProjectileFactory::Create(
		playerTransformCache->GetForwardDir(),
		offsetWorld,
		30
	);

	AE::System::AudioManager::GetInstance().PlayAudio("gunshot");
}

//TODO: Look into why the y axis is negative
void PlayerSystem::OnVertical(float y) {
	lookAxisDelta += {0, -y, 0};
}

void PlayerSystem::OnHorizontal(float x) {
	lookAxisDelta += {-x, 0, 0};
}

inline void PlayerSystem::modifyMovement(float x, float y, float z) {
	playerMovementCache->Dir += {x, y, z};
}
