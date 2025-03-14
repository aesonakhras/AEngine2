#pragma once

#include <memory>
#include <variant>

#include "btBulletDynamicsCommon.h"

#include "Core/Types.h"

#include "Math/Vec3.h"

namespace AE::Physics {
	enum class RigidBodyType {
		Static,
		Kinematic,
		Dynamic
	};

	enum class RigidBodyShape {
		PLANE,
		SPHERE,
		BOX,
		CUSTOM //Convex Hull
	};

	struct PlanePhysicsShapeCreateInfo
	{
		Vec3 Normal;
		float32 Offset;
	};

	struct SpherePhysicsShapeCreateInfo
	{
		float32 Radius;
	};

	struct  BoxPhysicsShapeCreateInfo
	{
		Vec3 HalfExtent;
	};

	struct ConvexHullShapeCreateInfo {
		std::shared_ptr<btConvexHullShape> convexHull;
	};

	struct RigidBodyCreateInfo
	{
		RigidBodyShape Shape;
		std::variant<
			PlanePhysicsShapeCreateInfo,
			SpherePhysicsShapeCreateInfo,
			BoxPhysicsShapeCreateInfo,
			ConvexHullShapeCreateInfo
		> ShapeCreateInfo;

		//parameters
		RigidBodyType Type;
		Vec3 StartPos;
		float32 Mass;
		Vec3 StartVelocity;
		float32 Friction;
		float32 Bounciness;
	};
}