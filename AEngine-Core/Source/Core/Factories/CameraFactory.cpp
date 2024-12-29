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
	Vec3 position,
	Vec3 lookAt
) {
	entt::entity entity = registry.create();

	registry.emplace<Camera>(entity, fov, aspectRatio, nearZ, farZ);
	
	Transform transform{};

	transform.SetPosition(position);

	auto lookDir = lookAt - position;

	lookDir = lookDir.Normalized();

	float yaw = std::atan2f(lookDir.X, lookDir.Z);
	float pitch = std::asinf(lookDir.Y);

	transform.SetRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({pitch, yaw, 0.0f}));
	
	registry.emplace<Transform>(entity);

	return entity;
}
