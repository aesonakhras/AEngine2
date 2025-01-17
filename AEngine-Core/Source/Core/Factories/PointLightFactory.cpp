#include <DirectXMath.h>

#include "PointLightFactory.h"
#include "Core/Components/PointLight.h"

using namespace AE::Graphics;


void PointLightFactory::AddToEntity(
	entt::registry& registry,
	entt::entity entity,
	Vec3 position,
	Transform* parent,
	float intensity,
	Vec3 color,
	float radius,
	float linear,
	float quad
) {
	//add a transform, then add the point light
	registry.emplace<Transform>(
			entity, 
			Transform {	
				entity,
				position,
				DirectX::XMQuaternionIdentity(),
				{1.0f, 1.0f, 1.0f},
				parent,
				"PointLight" 
			}
		);

	registry.emplace<PointLight>(
		entity,
		PointLight {
			intensity,
			color,
			radius,
			linear,
			quad
		}
	);
}