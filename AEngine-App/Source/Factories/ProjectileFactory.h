#pragma once
#include <DirectXMath.h>

#include "Core/Types.h"
#include "Math/Vec3.h"
#include "Core/Scene/SceneManager.h"

#include "Core/Factories/StaticMeshFactory.h"
#include "Core/Components/RigidBody.h"

#include "Core/Components/PointLight.h" 
#include "Resources/ResourceManager.h"

#include "Components/Projectile.h"

namespace AE::App {
	class ProjectileFactory {
	public:
		static entt::entity Create(Vec3 direction, Vec3 startPosition, float32 speed, std::string append = "") {
			auto projectileMesh = AE::Core::ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Sphere.obj"));

			auto projectileMaterial = AE::Core::ResourceManager::GetInstance().GetSharedMaterial("ProjectileMaterial");
			auto& sceneManager = AE::Core::SceneManager::GetInstance();

			auto bulletEntity = AE::Core::StaticMeshFactory::Create(
				sceneManager.Registry,
				*projectileMesh.get(),
				*projectileMaterial.get(),
				startPosition,
				DirectX::XMQuaternionIdentity(),
				{ 1.0f, 1.0f, 1.0f },
				entt::null,
				"Bullet" + append
			);

			sceneManager.Registry.emplace<Projectile>(bulletEntity, direction, speed);

			Vec3 color = { 1, 0.6, 0.6 };

			sceneManager.Registry.emplace<AE::Graphics::PointLight>(
					bulletEntity,
					5.0f,
					color,
					10.0f,
					0.09f,
					0.032f
				);

			AE::Physics::RigidBodyCreateInfo info = {
				AE::Physics::RigidBodyShape::SPHERE,
				AE::Physics::SpherePhysicsShapeCreateInfo(1.0f),
				AE::Physics::RigidBodyType::Dynamic,
				startPosition,
				0.25f,
				direction * speed,
				0.5f,
				1.0f
			};

			sceneManager.Registry.emplace<AE::Physics::RigidBody>(
				bulletEntity,
				//rigidbody
				bulletEntity,
				info);

			return bulletEntity;
		}
	};
}