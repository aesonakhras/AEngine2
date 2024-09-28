#pragma once
#include <DirectXMath.h>

struct Transform {
	//TODO: Need to abstract this out before too long
	//also quaternion?
	DirectX::XMVECTOR Position;
	DirectX::XMVECTOR Rotation;	//NOTE: This is a quaternion
	DirectX::XMVECTOR Scale;
	
	Transform() {
		Position = {0.0f,0.0f,0.f};
		Rotation = DirectX::XMQuaternionIdentity();
		Scale = { 1.0f,1.0f,1.0f };
	}

	Transform(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale) {
		Position = position;
		Rotation = rotation;
		Scale = scale;
	}

	DirectX::XMMATRIX ToMatrix() {
		return DirectX::XMMatrixScalingFromVector(Scale) *
			DirectX::XMMatrixRotationQuaternion(Rotation) *
			DirectX::XMMatrixTranslationFromVector(Position);
	}
};