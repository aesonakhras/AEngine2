#pragma once
#include <vector>
#include <random>

#include "entt/entt.hpp"

#include "Core/Components/Transform.h"

#include "Core/Systems/TransformSystem.h"
#include "Core/Systems/SystemLocator.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Debug.h"

#include "Components/Projectile.h"

namespace AE::App {
	class ProjectileSystem {
		std::mt19937 gen;

		public:
		ProjectileSystem()
			: gen(std::random_device{}()) {}


		void Update(float32 deltaTime,
			entt::registry& regsitry,
			AE::Core::JobSystem& jobSystem,
			AE::Core::CommandBuffer& commandBuffer
		) {
			jobSystem.SubmitJob([this, deltaTime, &regsitry, &commandBuffer]() {
				auto projectileView = regsitry.view<Projectile, Transform>();

				for (auto& projectileEntity : projectileView) {
					auto& projectile = regsitry.get<Projectile>(projectileEntity);
					auto& projectileTransform = regsitry.get<Transform>(projectileEntity);

					auto transformSystem = AE::Core::SystemLocator::Get<AE::Core::TransformSystem>();

					Vec3 position = projectileTransform.GetLocalPosition();

					position += projectile.Direction * projectile.Speed * deltaTime;

					transformSystem->SetLocalPosition(projectileEntity, position);
				}

				timeSinceLastDelete += deltaTime;

				if (timeSinceLastDelete > timeToDelete) {
					timeSinceLastDelete = 0;
					deleteRandomProjectile(regsitry);
				}
			});
		}

		private:
			float32 timeToDelete = 1.0f;
			float32 timeSinceLastDelete = 0.0f;

			void deleteRandomProjectile(entt::registry& regsitry) {
				auto projectileView = regsitry.view<Projectile>();

				if (projectileView.size() == 0) return;

				std::uniform_int_distribution<> dist(0, projectileView.size() - 1);

				int randomNumber = dist(gen);

				auto it = projectileView.begin();
				std::advance(it, randomNumber);
				entt::entity specificEntity = *it;

				AE::Core::SceneManager::GetInstance().DeleteEntity(specificEntity);
			}
	};
}