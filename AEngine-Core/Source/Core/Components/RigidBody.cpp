#include "RigidBody.h"

#include "Physics/PhysicsManager.h"

using namespace AE::Physics;

RigidBody::RigidBody(entt::entity entity, const RigidBodyCreateInfo& info) : Type(info.Type),
    collisionShapeBullet(nullptr),
    rigidBodyBullet(nullptr),
    motionStateBullet(nullptr) 
{

    switch (info.Shape)
    {
        case(RigidBodyShape::PLANE):
            HandleInfinitePlaneCreate(std::get<PlanePhysicsShapeCreateInfo>(info.ShapeCreateInfo));
            break;
        case(RigidBodyShape::SPHERE):
            HandleSphereCreate(std::get<SpherePhysicsShapeCreateInfo>(info.ShapeCreateInfo));
            break;
        case(RigidBodyShape::BOX):
            HandleBoxCreate(std::get<BoxPhysicsShapeCreateInfo>(info.ShapeCreateInfo));
        default:
            break;
    }

    //if (Type == RigidBodyType::Dynamic || Type == RigidBodyType::Kinematic) {
        motionStateBullet = std::make_unique<btDefaultMotionState>(
            btTransform(
                btQuaternion(0, 0, 0, 1),
                PhysicsManager::ToBulletVec3(info.StartPos)
            )
        );
    //}

    btVector3 Inertia(0, 0, 0);

    if (Type == RigidBodyType::Dynamic && info.Mass > 0.0f) {
        collisionShapeBullet->calculateLocalInertia(info.Mass, Inertia);
    }

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        info.Mass,
        motionStateBullet.get(),
        collisionShapeBullet.get(),
        Inertia
    );

    rigidBodyBullet = std::make_unique<btRigidBody>(rigidBodyCI);

    //if kinematic do some additional setup
    if (Type == RigidBodyType::Kinematic) {
        rigidBodyBullet->setCollisionFlags(rigidBodyBullet->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        rigidBodyBullet->setActivationState(DISABLE_DEACTIVATION);
    }
    else if (Type == RigidBodyType::Static) {
        rigidBodyBullet->setCollisionFlags(rigidBodyBullet->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    }

    if (Type == RigidBodyType::Dynamic || Type == RigidBodyType::Kinematic) {
        rigidBodyBullet->setLinearVelocity({ info.StartVelocity.X, info.StartVelocity.Y, info.StartVelocity.Z });
    }

    rigidBodyBullet->setFriction(info.Friction);
    rigidBodyBullet->setRestitution(info.Bounciness);

    rigidBodyBullet->setUserIndex(static_cast<int>(entity));

    //Add physice body to the world
    AE::Physics::PhysicsManager::GetInstance().dynamicsWorld.addRigidBody(rigidBodyBullet.get());
}

void RigidBody::HandleInfinitePlaneCreate(const AE::Physics::PlanePhysicsShapeCreateInfo& info) {
    //INFNITE PLANE btw
    collisionShapeBullet = std::make_unique<btStaticPlaneShape>(btVector3(info.Normal.X, info.Normal.Y, info.Normal.Z), info.Offset);
}

void RigidBody::HandleSphereCreate(const AE::Physics::SpherePhysicsShapeCreateInfo& info) {
    collisionShapeBullet = std::make_unique <btSphereShape>(info.Radius);
}

void RigidBody::HandleBoxCreate(const AE::Physics::BoxPhysicsShapeCreateInfo& info) {
    collisionShapeBullet = std::make_unique<btBoxShape>(
        btVector3(
            info.HalfExtent.X,
            info.HalfExtent.Y,
            info.HalfExtent.Z
        )
    );
}

RigidBody::~RigidBody() {
    if (rigidBodyBullet != nullptr) {
        AE::Physics::PhysicsManager::GetInstance().dynamicsWorld.removeRigidBody(rigidBodyBullet.get());
    }
}

Vec3 RigidBody::GetPosition() {
    auto transform = rigidBodyBullet->getWorldTransform();

    auto origin = transform.getOrigin();

    return { origin.x(), origin.y(), origin.z() };
}

void RigidBody::SetPosition(Vec3 pos) {
    btTransform transform = rigidBodyBullet->getWorldTransform();
    transform.setOrigin(btVector3(pos.X, pos.Y, pos.Z));
    rigidBodyBullet->setWorldTransform(transform);

    //incase it is sleeping
    rigidBodyBullet->activate();
}

bool RigidBody::IsStatic() {
    if (Type == RigidBodyType::Static) {
        return true;
    }

    return false;
}

void RigidBody::ApplyForce(const Vec3& force) {
    rigidBodyBullet->applyCentralForce({ force.X, force.Y, force.Z });
}

void RigidBody::ApplyForceAtPoint(const Vec3& force, const Vec3 Point) {
    rigidBodyBullet->applyForce({ force.X, force.Y, force.Z }, { Point.X, Point.Y, Point.Z });
}
