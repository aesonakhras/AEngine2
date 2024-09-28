#include <cmath>

#include "CameraFactory.h"
#include "Core/Common.h"

#include "Core/Components/Transform.h"
#include "Core/Components/Camera.h"

using namespace AE::Core;

entt::entity CameraFactory::Create(
	entt::registry& registry,
	float32 fov,
	float32 aspectRatio,
	float32 nearZ,
	float32 farZ,
	DirectX::XMVECTOR position,
	DirectX::XMVECTOR lookAt
) {
	entt::entity entity = registry.create();

	registry.emplace<Camera>(entity, fov, aspectRatio, nearZ, farZ);
	
	Transform transform{};

	transform.Position = position;

	//DirectX::XMMATRIX rotation = DirectX::XMMatrixLookAtLH(position, lookAt, { 0.0f, 1.0f, 0.0f });

	auto lookDir = DirectX::XMVectorSubtract(lookAt, position);

	lookDir = DirectX::XMVector3Normalize(lookDir);

	float x = DirectX::XMVectorGetX(lookDir);
	float y = DirectX::XMVectorGetX(lookDir);
	float z = DirectX::XMVectorGetX(lookDir);

	float yaw = std::atan2f(x, z);
	float pitch = std::asinf(y);

	transform.Rotation = DirectX::XMQuaternionRotationRollPitchYawFromVector({pitch, yaw, 0.0f});
	
	registry.emplace<Transform>(entity);

	return entity;
}
