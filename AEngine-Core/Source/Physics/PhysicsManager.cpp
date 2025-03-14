#include <iostream>

#include <memory>

#include "PhysicsManager.h"

using namespace AE::Physics;

PhysicsManager::PhysicsManager()
    : collisionConfig(),
    dispatcher(&collisionConfig),           
    broadphase(),                            
    solver(),
    dynamicsWorld(
        &dispatcher,
        &broadphase,
        &solver,
        &collisionConfig
    ) 
{
    dynamicsWorld.setGravity(btVector3(0, -9.8, 0));
}

void PhysicsManager::StepPhysics(float32 deltaTime, float32 stepAmount) {
    dynamicsWorld.stepSimulation(deltaTime, 10, stepAmount);
}

//DO NOT CALL THIS FUNCTION DIRECTLY ANYWHERE BUT CheckCollisionRigidBody(...)
btScalar PhysicsManager::addSingleResult(
    btManifoldPoint& cp,
    const btCollisionObjectWrapper* colObj0Wrap,
    int partId0,
    int index0,
    const btCollisionObjectWrapper* colObj1Wrap,
    int partId1,
    int index1
) {
    if (cp.getDistance() < 0) return 0;

    tracker->Add({
            colObj0Wrap->getCollisionObject(),
            colObj1Wrap->getCollisionObject(),
            cp.getPositionWorldOnA(),
            cp.getPositionWorldOnB()
        }
    );

    return 0;
}

void PhysicsManager::CheckCollisionRigidBody(
    btRigidBody& rigidBody,
    CollisionTracker& collisionTracker
) {

    //TODO: implement Broad phase and better stuff
    tracker = &collisionTracker;

    dynamicsWorld.contactTest(&rigidBody, *this);
}

std::shared_ptr<btConvexHullShape> PhysicsManager::CreateBounds(
    const MeshData& meshData,
    int vertexStride,
    AABB& Aabb
) {
    std::shared_ptr<btConvexHullShape> convexHullShape = std::make_shared<btConvexHullShape>();

    for (int i = 0; i < meshData.vertexCount; i+=3) {
        
        size_t offset = i * (vertexStride / sizeof(float));

        float* vertices = static_cast<float*>(meshData.vertexData);

        //convert vertex to btVertex
        btVector3 vert = {
            vertices[offset],
            vertices[offset + 1],
            vertices[offset + 2]
        };

        convexHullShape->addPoint(vert, false);
    }

    convexHullShape->optimizeConvexHull();
    convexHullShape->recalcLocalAabb();

    btVector3 min;
    btVector3 max;

    btTransform transform;

    convexHullShape->getAabb(transform, min, max);

    Aabb.min = {
        min.x(),
        min.y(),
        min.z()
    };

    Aabb.max = {
        max.x(),
        max.y(),
        max.z()
    };

    return convexHullShape;
}

bool PhysicsManager::initialize() {
    return true;
}

void PhysicsManager::shutdown() {

}

PhysicsManager::~PhysicsManager() {

}
