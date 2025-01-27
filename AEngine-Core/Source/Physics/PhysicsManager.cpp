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

bool PhysicsManager::initialize() {
    return true;
}

void PhysicsManager::shutdown() {

}

PhysicsManager::~PhysicsManager() {

}
