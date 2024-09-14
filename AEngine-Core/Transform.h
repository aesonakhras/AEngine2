#pragma once
#include <DirectXMath.h>

struct Transform {
	//TODO: Need to abstract this out before too long
	//also quaternion?
	DirectX::XMVECTOR Position;
	DirectX::XMVECTOR Rotation;	//euler
	DirectX::XMVECTOR Scale;
	
	Transform() {
		Position = {0.0f,0.0f,0.f};
		Rotation = { 0.0f,0.0f,0.f };
		Scale = { 1.0f,1.0f,1.0f };
	}

	Transform(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale) {
		Position = position;
		Rotation = rotation;
		Scale = scale;
	}
};