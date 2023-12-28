#pragma once
#include <DirectXMath.h>

struct Transform {
	//TODO: Need to abstract this out before too long
	DirectX::XMVECTOR Position;
	DirectX::XMVECTOR Rotation;	//euler
	DirectX::XMVECTOR Scale;
	
	Transform(DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale) {
		Position = position;
		Rotation = rotation;
		Scale = scale;
	}
};