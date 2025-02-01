#pragma once
#include <DirectXMath.h>
#include <memory>
#include <functional>
#include <entt/entt.hpp>

#include <btBulletCollisionCommon.h>

#include "Physics/RigidBodyCreateInfo.h"
#include "Physics/OnCollisionInfo.h"
#include "Math/Vec3.h"

namespace AE::Physics {
    struct RigidBody {
        RigidBody(entt::entity entity, const RigidBodyCreateInfo& info);
        ~RigidBody();

        RigidBodyType Type;
        RigidBodyShape Shape;
        Vec3 StartPos;
        float32 Mass;
        Vec3 Inertia;
        std::function<void(const OnCollisionInfo&)> OnCollision;

        Vec3 GetPosition();
        DirectX::XMVECTOR GetRotation();

        void SetPosition(Vec3 pos);
        bool IsStatic();
        void ApplyForce(const Vec3& force);
        void ApplyForceAtPoint(const Vec3& force, const Vec3 Point);

            //I HATE that this is here, but it is necessary evil
            std::unique_ptr<btRigidBody> rigidBodyBullet;
        private:
            std::unique_ptr<btCollisionShape> collisionShapeBullet;
            
            std::unique_ptr<btDefaultMotionState> motionStateBullet;

            void HandleInfinitePlaneCreate(const AE::Physics::PlanePhysicsShapeCreateInfo& info);
            void HandleSphereCreate(const AE::Physics::SpherePhysicsShapeCreateInfo& info);
            void HandleBoxCreate(const AE::Physics::BoxPhysicsShapeCreateInfo& info);
    };

}