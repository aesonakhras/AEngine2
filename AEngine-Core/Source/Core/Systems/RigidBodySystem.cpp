#include "RigidBodySystem.h"
#include "Core/Components/Transform.h"

#include "Core/Components/RigidBody.h"
#include "Physics/PhysicsManager.h"

#include "Math/Vec3.h"

using namespace AE::Physics;

void RigidBodySystem::Update(entt::registry& registry, float32 timeStep) {
	//Transforms
	auto rigidBodyTransformView = registry.view<Transform, RigidBody>();

	for (auto entity : rigidBodyTransformView) {
		auto& transform = rigidBodyTransformView.get<Transform>(entity);
		auto& rigidBody = rigidBodyTransformView.get<RigidBody>(entity);

		if (!rigidBody.IsStatic()) {
			//only need to update the world transform and not the parent matricies
			Vec3 rigidBodyPosition = rigidBody.GetPosition();

			//std::cout << "X: " << rigidBodyPosition.X << "Y: " << rigidBodyPosition.Y << "Z: " << rigidBodyPosition.Z << std::endl;

			transform.SetWorldPosition(rigidBodyPosition);
		}
	}

	//TODO: These should be more than doable in parallel

	//check for collisions
	for (auto entity : rigidBodyTransformView) {
		auto& transform = rigidBodyTransformView.get<Transform>(entity);
		auto& rigidBody = rigidBodyTransformView.get<RigidBody>(entity);
		
		PhysicsManager::GetInstance().CheckCollisionRigidBody(
			*(rigidBody.rigidBodyBullet.get()),
			collisionTracker
		);
	}

	//handle neccessary callbacks
	for (auto& rawCollisionInfo : collisionTracker.GetCollisions()) {
		//get the actual rigidbodies of the collisions
		auto* shape1 = rawCollisionInfo.second.objectA;
		auto* shape2 = rawCollisionInfo.second.objectB;

		entt::entity entity1 = static_cast<entt::entity>(shape1->getUserIndex());
		entt::entity entity2 = static_cast<entt::entity>(shape2->getUserIndex());

		auto& rigidBody1 = registry.get<RigidBody>(entity1);
		auto& rigidBody2 = registry.get<RigidBody>(entity2);

		OnCollisionInfo collisionInfo1 = {
			{
				rawCollisionInfo.second.contactPointA.x(),
				rawCollisionInfo.second.contactPointA.y(),
				rawCollisionInfo.second.contactPointA.z()
			},
			{0,0,0},
			entity2
		};

		OnCollisionInfo collisionInfo2 = {
			{
				rawCollisionInfo.second.contactPointA.x(),
				rawCollisionInfo.second.contactPointA.y(),
				rawCollisionInfo.second.contactPointA.z()
			},
			{0,0,0},
			entity1
		};

		if (rigidBody1.OnCollision) {
			rigidBody1.OnCollision(collisionInfo1);
		}

		if (rigidBody2.OnCollision) {
			rigidBody2.OnCollision(collisionInfo2);
		}
	}

	//can have two later on to determine exit and enter collision
	collisionTracker.Clear();
}
