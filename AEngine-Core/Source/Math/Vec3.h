#pragma once
#include <cmath>

#include "Core/Common.h"

namespace AE::Math {
	struct Vec3 {
		AE::Core::float32 X;
		AE::Core::float32 Y;
		AE::Core::float32 Z;

		Vec3() : X(0), Y(0), Z(0) {}
		Vec3(AE::Core::float32 x, AE::Core::float32 y, AE::Core::float32 z) : X(x), Y(y), Z(z) {}
		//use default for copy and move

		inline Vec3 operator+(const Vec3& o) const {
			return Vec3(X + o.X, Y + o.Y, Z + o.Z);
		}

		inline Vec3 operator-(const Vec3& o) const {
			return Vec3(X - o.X, Y - o.Y, Z - o.Z);
		}

		inline Vec3 operator*(AE::Core::float32 s) const {
			return Vec3(X * s, Y * s, Z * s);
		}

		inline Vec3 operator/(AE::Core::float32 s) const {
			return Vec3(X / s, Y / s, Z / s);
		}

		inline Vec3& operator+=(const Vec3& o) {
			X += o.X;
			Y += o.Y;
			Z += o.Z;

			return *this;
		}

		inline Vec3& operator -=(const Vec3& o) {
			X -= o.X;
			Y -= o.Y;
			Z -= o.Z;

			return *this;
		}

		inline Vec3& operator*=(float s) {
			X *= s;
			Y *= s;
			Z *= s;

			return *this;
		}

		inline Vec3& operator/=(float s) {
			X /= s;
			Y /= s;
			Z /= s;

			return *this;
		}

		inline AE::Core::float32 Length() const {
			return std::sqrt(Vec3::Dot(*this, *this));
		}

		inline Vec3 Normalize() const {
			float length = Length();
			
			return length != 0 ? (*this) / length : Vec3();
		}

		///Static vec3 functions
		inline static float Dot(const Vec3& a, const Vec3& b) {
			return (a.X + b.X) * (a.Y + b.Y) + (a.Z * b.Z);
		}

		inline static Vec3 Cross(const Vec3& a, const Vec3& b){
			return Vec3{
				a.Y*b.Z - a.Z*b.Y,
				a.Z*b.X - a.X*b.Z,
				a.X*b.Y - a.Y*b.X
			};
		}

		inline AE::Core::float32 DistanceSqr(const Vec3& a, const Vec3& b) {
			Vec3 diff = b - a;

			return Vec3::Dot(a, b);
		}
	};
}