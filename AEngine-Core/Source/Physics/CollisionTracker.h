#pragma once
#include <unordered_map>
#include <btBulletDynamicsCommon.h>

namespace AE::Physics {
	struct CollisionInfoRaw {
		const btCollisionObject* objectA;
		const btCollisionObject* objectB;
		btVector3 contactPointA;
		btVector3 contactPointB;
		btScalar distance;

		bool operator==(const CollisionInfoRaw& other) const {
			return (objectA == other.objectA && objectB == other.objectB) ||
				(objectA == other.objectB && objectB == other.objectA);
		}
	};

	struct CollisionPairHash {
		std::size_t operator()(const std::pair<const btCollisionObject*, const btCollisionObject*>& pair) const {
			auto hashA = std::hash<const btCollisionObject*>()(pair.first);
			auto hashB = std::hash<const btCollisionObject*>()(pair.second);
			return hashA ^ (hashB << 1);
		}
	};

	using CollisionMap = 
		std::unordered_map<
			std::pair<
				const btCollisionObject*,
				const btCollisionObject*
			>,
			CollisionInfoRaw,
			CollisionPairHash
		>;

	class CollisionTracker {
	public:
		void Add(
			const CollisionInfoRaw& info
		) {
			//TODO_E: Ensure that objectA != objectB

			//consistent order
			auto collisionPair = std::make_pair(
				info.objectA < info.objectB ? info.objectA : info.objectB,	//smaller address
				info.objectA > info.objectB ? info.objectA : info.objectB	//larger address
			);

			if (!CollisionTracker.contains(collisionPair)) {
				CollisionTracker[collisionPair] = info;
			}
		}

		const CollisionMap& GetCollisions() const {
			return CollisionTracker;
		}

		void Clear() {
			CollisionTracker.clear();
		}
	private:
		CollisionMap CollisionTracker;
	};
}