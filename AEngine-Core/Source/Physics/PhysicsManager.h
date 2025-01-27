#pragma once
#include <memory>
#include <vector>
#include <btBulletDynamicsCommon.h>

#include "Utils/Singleton.h"
#include "RigidBodyCreateInfo.h"
#include "Core/Components/RigidBody.h"
#include "Physics/CollisionTracker.h"

namespace AE::Physics {
	class PhysicsManager : public AE::Utils::Singleton<PhysicsManager>, public btCollisionWorld::ContactResultCallback {
		public:

			void StepPhysics(float32 deltaTime, float32 stepAmount);

			static btVector3 ToBulletVec3(Vec3 v) {
				return { v.X, v.Y, v.Z };
			}

			void CheckCollisionRigidBody(
				btRigidBody& rigidBody,
				CollisionTracker& collisionTracker
			);

			btDiscreteDynamicsWorld dynamicsWorld;

			//implementation of btCollisionWorld::ContactResultCallback
			btScalar addSingleResult(
				btManifoldPoint& cp,
				const btCollisionObjectWrapper* colObj0Wrap,
				int partId0,
				int index0,
				const btCollisionObjectWrapper* colObj1Wrap,
				int partId1,
				int index1
			) override;

		protected:
			bool initialize();
			void shutdown();

		private:
			PhysicsManager();
			~PhysicsManager();

			friend class AE::Utils::Singleton<PhysicsManager>;

			btDefaultCollisionConfiguration collisionConfig;
			btCollisionDispatcher dispatcher;
			btDbvtBroadphase broadphase;
			btSequentialImpulseConstraintSolver solver;

			CollisionTracker* tracker = nullptr;
	};
}