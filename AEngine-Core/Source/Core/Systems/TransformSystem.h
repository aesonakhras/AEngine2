#pragma once
#include <DirectXMath.h>

#include "entt/entt.hpp"

#include "System/MultiThreading/CommandBuffer.h"
#include "Math/Vec3.h"
#include "Core/Components/Transform.h"

namespace AE::Core {
	class TransformSystem {
		public:
			TransformSystem();

			void SetScene(entt::registry& registry);

			void SetLocalPosition(entt::entity entity, Vec3 newPos);

			Vec3 GetLocalPosition(entt::entity transform);

			void SetLocalRotation(entt::entity entity, DirectX::XMVECTOR newRot);

			DirectX::XMVECTOR GetLocalRotation(entt::entity entity);

			void Update();

		private:
			CommandBuffer transformCommandbuffer;
			entt::registry* registry;

			void updateWorldMatrix(
				Transform* parentTransform,
				DirectX::XMMATRIX parentWorldMatrix, 
				bool parentDirtyStatus
			);

			
	};
}