#pragma once
#include <vector>


#include "entt/entt.hpp"

#include "Core/Components/Transform.h"

#include "Core/Systems/TransformSystem.h"
#include "Core/Systems/SystemLocator.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Debug.h"

#include "Components/Projectile.h"

namespace AE::App {
	class ProjectileSystem {


		public:
		ProjectileSystem() {}


		void Update(float32 deltaTime,
			entt::registry& regsitry,
			AE::Core::JobSystem& jobSystem,
			AE::Core::CommandBuffer& commandBuffer
		) {
			
			auto projectileView = regsitry.view<Projectile, Transform>();

			for (auto& projectileEntity : projectileView) {
				jobSystem.SubmitJob([this, deltaTime, &projectileView, &commandBuffer, projectileEntity]() {
					auto& projectile = projectileView.get<Projectile>(projectileEntity);
					auto& projectileTransform = projectileView.get<Transform>(projectileEntity);

					auto transformSystem = AE::Core::SystemLocator::Get<AE::Core::TransformSystem>();

					Vec3 position = projectileTransform.GetLocalPosition();

					position += projectile.Direction * projectile.Speed * deltaTime;

					transformSystem->SetLocalPosition(projectileEntity, position);


					projectile.TimeSinceSpawn += deltaTime;

					if (projectile.TimeSinceSpawn >= projectileDuration) {
						AE::Core::SceneManager::GetInstance().DeleteEntity(projectileEntity);
					}
				});
			}
			
		}

	private:
		float32 projectileDuration = 10.0f;
	};
}