#include <cmath>

#include "CameraFactory.h"
#include "Core/Common.h"

#include "Core/Components/Transform.h"
#include "Core/Components/Camera.h"

#include "Core/Scene/SceneManager.h"

using namespace AE::Core;

entt::entity CameraFactory::Create(
	entt::registry& registry,
	float32 fov,
	float32 aspectRatio,
	float32 nearZ,
	float32 farZ,
	bool isOrthographic,
	float32 orthographicSize,
	Vec3 position,
	Vec3 lookAt,
	bool isMainCamera
) {
	entt::entity entity = registry.create();

	registry.emplace<Camera>(
		entity,
		fov,
		aspectRatio,
		nearZ,
		farZ,
		isOrthographic,
		orthographicSize
	);

	if (isMainCamera) {
		//set this in the scene manager
		//TODO: ERROR if multiple or none are set
		
		if (SceneManager::GetInstance().mainCameraEntity == entt::null) {
			SceneManager::GetInstance().mainCameraEntity = entity;
		}
		else {
			AE::Core::Debug::LogWarning("Multiple Main cameras set");
		}
		
	}
	

	auto lookDir = lookAt - position;

	lookDir = lookDir.Normalized();

	float yaw = std::atan2f(lookDir.X, lookDir.Z);
	float pitch = std::asinf(lookDir.Y);
	
	Transform transform {
		entity,
		position,
		DirectX::XMQuaternionRotationRollPitchYawFromVector({pitch, yaw, 0.0f}),
		{ 1.0f, 1.0f, 1.0f },
		entt::null,
		"Camera"
	};

	registry.emplace<Transform>(entity, transform);

	return entity;
}
