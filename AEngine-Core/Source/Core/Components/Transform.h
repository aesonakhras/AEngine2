#pragma once
#include "Math/Vec3.h"
#include <DirectXMath.h>


//TODO: implement matrix to get rid of the directX internal dependency
struct Transform {
	Transform() {
		Position = {0.0f,0.0f,0.f};
		Rotation = DirectX::XMQuaternionIdentity();
		Scale = { 1.0f,1.0f,1.0f };
		isDirty = false;
	}

	Transform(Vec3 pos, DirectX::XMVECTOR rot, Vec3 scale) {
		Position = { pos.X, pos.Y, pos.Z, 1.0f };
		Rotation = rot;
		Scale = { scale.X, scale.Y, scale.Z };
		isDirty = false;
	}

	void SetPosition(Vec3 pos) {
		Position = { pos.X, pos.Y, pos.Z, 1.0f };
		isDirty = true;
	}

	Vec3 GetPosition() {
		return { 
			DirectX::XMVectorGetX(Position),
			DirectX::XMVectorGetY(Position), 
			DirectX::XMVectorGetZ(Position)
		};
	}

	void SetRotation(DirectX::XMVECTOR rot) {
		Rotation = rot;
		isDirty = true;
	}

	DirectX::XMVECTOR GetRotation() {
		return Rotation;
	}

	void SetScale(Vec3 scale) {
		Scale = { scale.X, scale.Y, scale.Z };
		isDirty = true;
	}

	Vec3 GetScale() {
		return {
			DirectX::XMVectorGetX(Scale),
			DirectX::XMVectorGetY(Scale),
			DirectX::XMVectorGetZ(Scale)
		};
	}

	//DO not use these in the application layer, you will break everything
	bool GetIsDirty() {
		return isDirty;
	}

	//DO not use these in the application layer, you will break everything
	bool resetIsDirty() {
		isDirty = true;
	}

	DirectX::XMMATRIX ToMatrix() {
		return DirectX::XMMatrixScalingFromVector(Scale) *
			DirectX::XMMatrixRotationQuaternion(Rotation) *
			DirectX::XMMatrixTranslationFromVector(Position);
	}

	private:
		//TODO: Need to abstract this out before too long
		//also quaternion?
		DirectX::XMVECTOR Position;
		DirectX::XMVECTOR Rotation;	//NOTE: This is a quaternion
		DirectX::XMVECTOR Scale;

		bool isDirty;
};