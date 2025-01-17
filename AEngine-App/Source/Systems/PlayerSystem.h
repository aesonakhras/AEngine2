#pragma once

#include <entt/entt.hpp>
#include <Core/Common.h>

#include "Math/Vec3.h"
#include "System/MultiThreading/JobSystem.h"
#include "System/MultiThreading/CommandBuffer.h"
#include "Core/Systems/TransformSystem.h"

//forward declare
struct Transform;

namespace AE::App {
	struct Movement;
	struct Player;


	class PlayerSystem {
		public:
			void Update(float32 deltaTime,
				entt::registry& scene,
				AE::Core::JobSystem& jobSystem,
				AE::Core::CommandBuffer& commandBuffer
			);

			void Start(entt::registry& scene);

		private:
			void UpdateRotation(AE::Core::TransformSystem& transformSystem, entt::entity playerEntity);
			void UpdateTranslation(
				AE::Core::TransformSystem& transformSystem,
				entt::entity playerEntity,
				Movement& playerMovement,
				float32 deltaTime,
				Player& player,
				Transform& transform
			);

			//TODO: Refactor input system to avoid this mess
			void OnForwardDown();
			void OnForwardUp();

			void OnBackwardDown();
			void OnBackwardUp();

			void OnRightDown();
			void OnRightUp();

			void OnLeftDown();
			void OnLeftUp();

			void OnQDown();
			void OnQUp();

			void OnZDown();
			void OnZUp();

			void OnSpaceDown();

			void OnVertical(float y);
			void OnHorizontal(float x);

			inline void modifyMovement(float x, float y, float z);

			entt::registry* m_scene;
			Movement* playerMovementCache;
			Transform* playerTransformCache;

			Vec3 lookAxisDelta{};
			float32 currPitch;
			float32 currYaw;
	};
}